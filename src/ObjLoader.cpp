#include "ObjLoader.h"

ObjLoader::ObjLoader()
{

}

ObjLoader::~ObjLoader()
{
	
}

void ObjLoader::Load(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("file not found\n");
		return;
	}

	int currentVertexSize = 0;
	int currentTriangleSize = 0;

	while (true)
	{
		char currentStr[128];
		int res = fscanf(file, "%s", currentStr);
		if (res == EOF)
		{
			break;
		}

		if (strcmp(currentStr, "o") == 0)
		{	
			vertexAcc.push_back(currentVertexSize);
			triangleAcc.push_back(currentTriangleSize);
		}

		if (strcmp(currentStr, "v") == 0)
		{
			Vector3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
			currentVertexSize++;
		}
		
		if (strcmp(currentStr, "vn") == 0)
		{
			Vector3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}

		else if (strcmp(currentStr, "f") == 0)
		{
			int vertexIndex[3];
			int normalIndex; //trash

			fscanf(file, "%i//%i %i//%i %i//%i\n", &vertexIndex[0], &normalIndex, &vertexIndex[1], &normalIndex, &vertexIndex[2], &normalIndex);
			triangles.push_back(vertexIndex[0] - 1);
			triangles.push_back(vertexIndex[1] - 1);
			triangles.push_back(vertexIndex[2] - 1);
			faceNormals.push_back(normals[normalIndex - 1]);
			currentTriangleSize += 3;
		}
	}
	fclose(file);
	vertexAcc.push_back(currentVertexSize);
	triangleAcc.push_back(currentTriangleSize);
	CalculateNormalPerVertex();
}

void ObjLoader::CalculateNormalPerVertex()
{
	normals.clear();
	normals.resize(vertices.size());

	for (int i = 0; i < triangles.size(); i += 3)
	{
		int v1 = triangles[i];
		int v2 = triangles[i + 1];
		int v3 = triangles[i + 2];

		Vector3 normal = faceNormals[i / 3];

		normals[v1] += normal;
		normals[v2] += normal;
		normals[v3] += normal;
	}

	for (int i = 0; i < normals.size(); i++)
	{
		normals[i].Normalize();
	}
}


Vector3* ObjLoader::GetVertices()
{
	return vertices.data();
}

int* ObjLoader::GetTriangles()
{
	return triangles.data();
}