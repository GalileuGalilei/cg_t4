#ifndef CONCAVE_POLYGON
#define CONCAVE_POLYGON

#include "PolygonShape.h"

class ConcavePolygon : public PolygonShape
{
public:

	ConcavePolygon(std::vector<Vector2> points) : PolygonShape(points) 
	{
		if (!IsValiblePolygon())
		{
			this->points[0] = std::vector<float>();
			this->points[1] = std::vector<float>();

			printf("ERRO: poligono inv�lido (auto intersecta)");
			return;
		}

		Triangulate();
	}

	/// <summary>
	/// insere os indices dos triangulos no vetor triangles usando o algoritmo ear-clipping
	/// </summary>
	bool Triangulate();

protected:

	std::vector<int> triangles;

	/// <summary>
	/// verifica se o ponto pode ser adicionado ao poligono sem causar auto intersec��o
	/// </summary>
	bool IsValiblePoint(Vector2 point);

	/// <summary>
	/// Renderiza os triangulos dos poligonos
	/// </summary>
	void RenderTriangles();

private:

	/// <summary>
	/// verifica se o poligono em quest�o � valido(n�o se auto intersecta)
	/// </summary>
	bool IsValiblePolygon();

	/// <summary>
	/// tenta achar o triangulo v�lido mais pr�ximo de i2, sendo o triangulo formador pelos v�rtices (i1,i2,i3)
	/// </summary>
	bool FindValibleTriangle(int* i1, int* i3, int i2, std::vector<bool> ignoreIndex);

	void OnRender(OnRenderEvent* args) override
	{
		RenderTriangles();
	}
};


#endif // ! CONCAVE_POLYGON
