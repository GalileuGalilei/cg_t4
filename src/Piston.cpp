#include "Piston.h"

Piston::Piston()
{
	ObjLoader loader;
	loader.Load("piston.obj");

	Mesh* meshes[5]{nullptr};

	for (int i = 0; i < loader.vertexAcc.size() - 1; i++)
	{
		meshes[i] = new Mesh();

		for (int j = loader.vertexAcc[i]; j < loader.vertexAcc[i + 1]; j++)
		{
			Vertex v;
			v.position = Vector4(loader.vertices[j].x, loader.vertices[j].y, loader.vertices[j].z, 1);
			v.normal = Vector4(loader.normals[j].x, loader.normals[j].y, loader.normals[j].z, 0);
			meshes[i]->vertices.push_back(v);
		}

		for (int j = loader.triangleAcc[i]; j < loader.triangleAcc[i + 1]; j += 3)
		{
			meshes[i]->triangles.push_back(Triangle(loader.triangles[j], loader.triangles[j + 1], loader.triangles[j + 2]));
		}
	}
	/*
	Float4x4 aux;
	aux.GeneratePerpectiveMatrix(200, 100);
	transform = transform * aux;

	aux.GenerateTranslationMatrix(200, 250, -250);
	transform = transform * aux;

	aux.GenerateScaleMatrix(50);
	transform = transform * aux;

	aux.GenerateRotationMatrix(Vector3(0, 1, 0), 0.1);
	transform = transform * aux;
	*/
	piston = meshes[0];
	piston->transform = transform;
	bottonAxis = meshes[1];
	bottonAxis->transform = transform;
	topAxis = meshes[2];
	topAxis->transform = transform;
	vertical = meshes[3];
	vertical->transform = transform;
	volant = meshes[4];
	volant->transform = transform;
}

Piston::~Piston()
{
}

void Piston::OnRender(OnRenderEvent* args)
{
	piston->Draw();
	bottonAxis->Draw();
	topAxis->Draw();
	vertical->Draw();
	volant->Draw();
}
