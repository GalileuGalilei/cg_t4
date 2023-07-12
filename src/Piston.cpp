#include "Piston.h"
#include <thread>
#include "GameManager.h"

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
	generalTransform = generalTransform * aux;

	aux.GenerateTranslationMatrix(370, 300, -250);
	generalTransform = generalTransform * aux;

	aux.GenerateScaleMatrix(50);
	generalTransform = generalTransform * aux;
	
	volantCenter = meshes[3]->GetCenter();
	topAxisCenter = meshes[4]->GetCenter();
	bottonAxisCenter = meshes[2]->GetCenter();
	
	rightPiston = meshes[0];
	rightVertical = meshes[1];
	bottonAxis = meshes[2];
	volant = meshes[3];
	topAxis = meshes[4];

	leftPiston = rightPiston->MakeCopy();
	leftVertical = rightVertical->MakeCopy();

	for (Vertex v : meshes[1]->vertices)
	{
		if (v.position.y > verticalBotton.y)
		{
			verticalBotton.y = v.position.y;
		}

		if (v.position.x > verticalBotton.x)
		{
			verticalBotton.x = v.position.x;
		}
	}
}

Piston::~Piston()
{
}

void Piston::OnRender(OnRenderEvent* args)
{
	rightPiston->Draw();
	rightVertical->Draw();
	leftPiston->Draw();
	leftVertical->Draw();
//	bottonAxis->Draw();
	volant->Draw();
	topAxis->Draw();
}



void Piston::OnUpdate(OnUpdateEvent* args)
{
	currentAngle += 5 * GameManager::deltaTime;
	Float4x4 rot;
	rot.GenerateRotationMatrix(volantCenter, Vector3(0, 0, 1), currentAngle);


	Vector4 aux = Vector4(topAxisCenter.x, topAxisCenter.y, topAxisCenter.z, 1);
	Vector2 dir = Vector2(verticalBotton.x, verticalBotton.y) - Vector2(topAxisCenter.x, topAxisCenter.y);
	Vector2 disp = Vector2(0, 0);
	Vector2 newAux = dir;
	dir.normalize();
	disp = Vector2(aux.x, aux.y);
	aux = rot * aux;
	disp = disp - aux;
	disp = disp.ortoProject(dir) * (-1);
	dir = Vector2(dir.y, -dir.x);

	dir = Vector2(verticalBotton.x + disp.x, verticalBotton.y + disp.y) - Vector2(aux.x, aux.y);
	float verticalAngle = -dir.Angle(newAux);
	aux = verticalBotton;
	aux.x += disp.x;
	aux.y += disp.y;
	
	Float4x4 pistonTrans;
	pistonTrans.GenerateTranslationMatrix(disp.x, disp.y, 0);

	Float2x2 dispRot;
	dispRot.GenerateRotationMatrix(verticalAngle);
	//disp = dispRot * disp;

	
	Float4x4 verticalRot;
	verticalRot.GenerateRotationMatrix(aux, Vector3(0, 0, 1), verticalAngle);
	Float4x4 verticalTrans;
	verticalTrans.GenerateTranslationMatrix(disp.x, disp.y, 0);
	
	Float4x4 leftsCorrection;
	leftsCorrection.GenerateRotationMatrix(topAxisCenter, Vector3(0, 0, 1), PI / 2);

	
	rightPiston->transform = generalTransform * pistonTrans;
	rightVertical->transform = generalTransform * (verticalRot * verticalTrans);
	bottonAxis->transform = generalTransform;
	volant->transform = generalTransform * rot;
	topAxis->transform = generalTransform * rot;

	verticalRot.GenerateRotationMatrix(aux, Vector3(0, 0, 1), -verticalAngle);
	verticalTrans.GenerateTranslationMatrix(disp.x, disp.y, 0);

	Vector4 verticalBotton2 = leftsCorrection * verticalBotton2;

	aux = Vector4(topAxisCenter.x, topAxisCenter.y, topAxisCenter.z, 1);
	dir = Vector2(verticalBotton2.x, verticalBotton2.y) - Vector2(topAxisCenter.x, topAxisCenter.y);
	disp = Vector2(0, 0);
	newAux = dir;
	dir.normalize();
	disp = Vector2(aux.x, aux.y);
	aux = rot * aux;
	disp = disp - aux;
	disp = disp.ortoProject(dir) * (-1);
	dir = Vector2(dir.y, -dir.x);

	dir = Vector2(verticalBotton2.x + disp.x, verticalBotton2.y + disp.y) - Vector2(aux.x, aux.y);
	verticalAngle = -dir.Angle(newAux);
	aux = verticalBotton2;
	aux.x += disp.x;
	aux.y += disp.y;

	pistonTrans.GenerateTranslationMatrix(disp.x, disp.y, 0);
	dispRot.GenerateRotationMatrix(verticalAngle);
	//disp = dispRot * disp;


	verticalRot;
	verticalRot.GenerateRotationMatrix(aux, Vector3(0, 0, 1), verticalAngle);
	verticalTrans;
	verticalTrans.GenerateTranslationMatrix(disp.x, disp.y, 0);

	
	leftPiston->transform = generalTransform * leftsCorrection * pistonTrans;
	leftVertical->transform =  generalTransform * leftsCorrection * (verticalRot * verticalTrans);
}