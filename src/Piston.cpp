#include "Piston.h"
#include <thread>

Piston::Piston()
{
	ObjLoader loader;
	loader.Load("piston.obj");

	Mesh* meshes[5];

	for (int i = 0; i < loader.vertexAcc.size() - 1; i++)
	{
		meshes[i] = new Mesh();
		meshes[i]->color = Vector3(1, 1, 1);

		for (int j = loader.vertexAcc[i]; j < loader.vertexAcc[i + 1]; j++)
		{
			Vertex v;
			v.position = Vector4(loader.vertices[j].x, loader.vertices[j].y, loader.vertices[j].z, 1);
			v.normal = Vector4(loader.normals[j].x, loader.normals[j].y, loader.normals[j].z, 0);
			meshes[i]->vertices.push_back(v);
		}

		for (int j = loader.triangleAcc[i]; j < loader.triangleAcc[i + 1]; j += 3)
		{
			int va = loader.vertexAcc[i];
			meshes[i]->triangles.push_back(Triangle(loader.triangles[j] - va, loader.triangles[j + 1] - va, loader.triangles[j + 2] - va));
		}
	}
	
	Float4x4 aux;
	aux.GeneratePerpectiveMatrix(200, 100);
	transform = transform * aux;

	aux.GenerateTranslationMatrix(300, 300, -250);
	transform = transform * aux;

	//todo: fix this
	bottonAxisCenter = meshes[2]->GetCenter();
	volantCenter = meshes[3]->GetCenter();
	topAxisCenter = meshes[4]->GetCenter();

	aux.GenerateScaleMatrix(50);
	transform = transform * aux;

	aux.GenerateRotationMatrix(Vector3(0, 1, 0), -1.65);
	transform = transform * aux;

	aux.GenerateRotationMatrix(volantCenter, Vector3(1, 0, 0), 0.5);
	topAxisTransform = aux;
	volantTransform = aux;
	
	piston = meshes[0];
	piston->transform = transform;
	vertical = meshes[1];
	vertical->transform = transform;
	bottonAxis = meshes[2];
	bottonAxis->transform = transform;
	volant = meshes[3];
	volant->transform = transform;
	topAxis = meshes[4];
	topAxis->transform = transform;

	for (Vertex v : meshes[1]->vertices)
	{
		if (v.position.y > verticalBotton.y)
		{
			verticalBotton.y = v.position.y;
		}
	}

	
}

Piston::~Piston()
{
}

void Piston::OnRender(OnRenderEvent* args)
{
	piston->Draw();
	vertical->Draw();
//	bottonAxis->Draw();
	volant->Draw();
	topAxis->Draw();
}

void Piston::OnUpdate(OnUpdateEvent* args)
{
	Vector2 disp = Vector2(0, 0);
	Vector2 dir = Vector2(bottonAxisCenter.x, bottonAxisCenter.y) - Vector2(volantCenter.x, volantCenter.y);

	Vector4 aux = Vector4(topAxisCenter.x, topAxisCenter.y, topAxisCenter.z, 1);
	aux = topAxis->transform * aux;
	disp = Vector2(aux.x, aux.y);
	topAxis->transform = topAxis->transform * topAxisTransform;

	aux = Vector4(topAxisCenter.x, topAxisCenter.y, topAxisCenter.z, 1);
	aux = topAxis->transform * aux;
	disp = Vector2(aux.x, aux.y) - disp;
	dir = Vector2(-dir.y, dir.x);
	dir.normalize();
	disp = disp.ortoProject(dir);
	dir = Vector2(bottonAxisCenter.x, bottonAxisCenter.y) - Vector2(volantCenter.x, volantCenter.y);
	float angle = dir.Angle(dir + disp);
	disp = Vector2(-disp.x, disp.y);

	


	//tá certo, não mexe

	pistonTransform.c.set(0, 0, 1, disp.x/50);
	pistonTransform.b.set(0, 1, 0, disp.y/50);

	Float4x4 temp;
	temp.GenerateTranslationMatrix(0, -currentHeight, 0);
	verticalTransform = temp;
	temp.GenerateRotationMatrix(bottonAxisCenter, Vector3(1, 0, 0), (angle)*0.1);
	verticalTransform = verticalTransform * temp;
	temp.GenerateTranslationMatrix(0, currentHeight, 0);
	verticalTransform = verticalTransform * temp;
	verticalTransform.b.w += disp.y / 50;
	currentHeight += disp.y / 50;
	
	

	volant->transform = volant->transform * volantTransform;
	piston->transform = piston->transform * pistonTransform;
	vertical->transform = vertical->transform * verticalTransform;
}