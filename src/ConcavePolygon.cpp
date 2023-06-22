#include "ConcavePolygon.h"
#include <vector>

void ConcavePolygon::RenderTriangles()
{
	float* x = points[0].data();
	float* y = points[1].data();

	if (tam < 4)
	{
		CV::polygon(x, y, tam);
		return;
	}

	int triangulesTam = triangles.size();
	float triangle[2][3]{};

	for (int i = 0; i < triangulesTam; i += 3)
	{
		triangle[0][0] = points[0][triangles[i]];
		triangle[0][1] = points[0][triangles[i + 1]];
		triangle[0][2] = points[0][triangles[i + 2]];

		triangle[1][0] = points[1][triangles[i]];
		triangle[1][1] = points[1][triangles[i + 1]];
		triangle[1][2] = points[1][triangles[i + 2]];

		CV::color(color.r, color.g, color.b);
		CV::polygonFill(triangle[0], triangle[1], 3);

	}
}

bool ConcavePolygon::IsValiblePoint(Vector2 point)
{
	if (tam < 3)
	{
		return true;
	}

	//possível nova linha
	Vector2 lst = Vector2(points[0][points[0].size() - 1], points[1][points[1].size() - 1]);
	Vector2 fst = Vector2(points[0][0], points[1][0]);

	Vector2 dir1 = fst - point;
	Vector2 dir2 = lst - point;

	dir1.normalize();
	dir2.normalize();

	Vector2 p1 = point + dir1;
	Vector2 p2 = fst - dir1;
	Vector2 p3 = point + dir2;
	Vector2 p4 = lst - dir2;

	return !(LineIntersectingCount(p1, p2, NULL) > 0 || 
			 LineIntersectingCount(p3, p4, NULL) > 0);
}

bool ConcavePolygon::IsValiblePolygon()
{
	if (tam < 3)
	{
		return true;
	}

	for (int i = 0; i < tam; i++)
	{
		if (!IsValiblePoint(Vector2(points[0][i], points[1][i])))
		{
			return false;
		}
	}
	return true;
}

//acha dois vertices disponíveis que possam formar um triangulo com i2
bool ConcavePolygon::FindValibleTriangle(int* i1, int* i3, int i2, std::vector<bool> ignoreIndex)
{
	*i3 = i2;
	*i1 = i2;

	while (true)
	{
		if (ignoreIndex[*i3] || *i3 == i2)
		{
			*i3 = (*i3 + 1) % tam;
			continue;
		}

		if (ignoreIndex[*i1] || *i1 == i2)
		{
			(*i1)--;

			if (*i1 < 0)
			{
				*i1 = tam - 1;
			}

			continue;
		}

		if (*i1 == *i3)
		{
			printf("ERRO: não há vertices disponíveis o suficiente para criar um triângulo");

			*i3 = 0;
			*i1 = 0;

			return false;
		}

		return true;
	}
}

bool ConcavePolygon::Triangulate() //implementação usando o algoritmo ear-clipping
{
	if (tam < 4)
	{
		return true;
	}

	int valiblePoints = tam;
	triangles.clear();

	std::vector<bool> ignoreIndex;
	for (int i = 0; i < tam; i++)
	{
		ignoreIndex.push_back(false);
	}

	int i2 = 0;

	while (valiblePoints > 3)
	{
		if (ignoreIndex[i2])
		{
			i2 = (i2 + 1) % tam;
			continue;
		}

		int i1 = 0;
		int i3 = 0;

		if (!FindValibleTriangle(&i1, &i3, i2, ignoreIndex))
		{
			return false;
		}

		Vector2 p1 = Vector2(points[0][i1], points[1][i1]);
		Vector2 p2 = Vector2(points[0][i2], points[1][i2]);
		Vector2 p3 = Vector2(points[0][i3], points[1][i3]);
		Vector2 dir;

		if (Orientation(p1, p3, Vector2(points[0][i2], points[1][i2])) == 0) 
		{
			i2 = (i2 + 1) % tam;
			continue;
		}

		if (p1 == p3 || p1 == p2 || p3 == p2)
		{
			i2 = (i2 + 1) % tam;
			continue;
		}

		dir = p1 - p3;
		dir.normalize();

		if (!LineToPolygon(p1 - dir, p3 + dir, &ignoreIndex)) //significa que a linha está totalmente dentro do polígono
		{
			i2 = (i2 + 1) % tam;
			continue;
		}

		valiblePoints--;

		triangles.push_back(i1);
		triangles.push_back(i2);
		triangles.push_back(i3);

		ignoreIndex[i2] = true;
		i2 = (i2 + 1) % tam;
	}

	for (int i = 0; i < tam; i++)
	{
		if (!ignoreIndex[i])
		{
			triangles.push_back(i);
		}
	}

	return true;
}