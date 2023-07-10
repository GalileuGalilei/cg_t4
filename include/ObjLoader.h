#pragma once
#include "Vectors.h"
#include <vector>

class ObjLoader
{
private:
	void CalculateNormalPerVertex();

public:
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector3> faceNormals;
	std::vector<int> triangles; // 3 indices per triangle
	std::vector<int> vertexAcc;
	std::vector<int> triangleAcc;

	ObjLoader();
	~ObjLoader();
	void Load(const char* filename);
	Vector3* GetVertices();
	int* GetTriangles();
};

