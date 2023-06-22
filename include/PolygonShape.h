#ifndef POLYGON_SHAPE
#define POLYGON_SHAPE

#include "Vectors.h"
#include "gl_canvas2d.h"
#include <vector>
#include "GameEvents.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// obs: foi usado o nome polygonShape para evitar conflitos com a classe Polygon do OpenGL //
/////////////////////////////////////////////////////////////////////////////////////////////

struct PolygonData
{
	int tam;
	Color color;
	bool isFilled;
	bool isCenterCalculated;
	Vector2 center;

	PolygonData()
	{
		tam = 0;
		isFilled = true;
		isCenterCalculated = false;
		center = Vector2(0, 0);
		color = Colors::black;
	}

	PolygonData(int tam, bool isFilled, bool isCenterCalculated, Vector2 center, Color color)
	{
		this->tam = tam;
		this->color = color;
		this->isFilled = isFilled;
		this->isCenterCalculated = isCenterCalculated;
		this->center = center;
	}
};

/// <summary>
/// classe que define uma base para polygonos quaisquer
/// </summary>
class PolygonShape : public IRenderable
{
protected:
	std::vector<float> points[2]; // [0] = x; [1] = y;
	Vector2 center;
	Color color = Colors::black;
	int tam = 0;
	bool isFilled = true;
	bool isRenderEnabled = true;
	bool isCenterCalculated = false;

public:

	void Translate(Vector2 vector);
	void SetPosition(Vector2 pos);
	void Rotate(float angle);
	void Rotate(float angle, Vector2 center);
	void Scale(Vector2 scale);
	void Scale(Vector2 scale, Vector2 center);
	void SetColor(Color color);
	void SetColor(float r, float g, float b);
	void SetRender(bool state);
	void Fill();
	Vector2 GetCenter();

	PolygonShape(std::vector<Vector2> points);

	/// <summary>
	/// cria um polígono no formato de um retângulo
	/// </summary>
	PolygonShape(Vector2 position, Vector2 size);

	/// <summary>
	/// 
	/// </summary>
	void LoadData(PolygonData data, std::vector<float> x, std::vector<float> y);

	/// <summary>
	/// verifica se uma linha está dentro do polígono
	/// </summary>
	bool LineToPolygon(Vector2 p1, Vector2 p2, std::vector<bool>* ignoreIndex);

	/// <summary>
	/// conta o número de interseções entre uma linha e o polígono
	/// </summary>
	int LineIntersectingCount(Vector2 p1, Vector2 q1, std::vector<bool>* ignoreIndex);

	/// <summary>
	/// verifica se um ponto está dentro do polígono
	/// </summary>
	bool PointToPolygon(Vector2 point, std::vector<bool>* ignoreIndex);

	int Orientation(Vector2 p1, Vector2 p2, Vector2 p3);

	bool IsLineIntersecting(Vector2 p1, Vector2 p2, Vector2 q1, Vector2 q2);

private:

	/// <summary>
	/// calcula a posição central do polígono
	/// </summary>
	void CalculateCenter();

	void OnRender(OnRenderEvent* args) override
	{
		if (!isRenderEnabled)
		{
			return;
		}

		float* x = points[0].data();
		float* y = points[1].data();

		CV::color(color.r, color.g, color.b);
		if (isFilled)
		{
			CV::polygonFill(x, y, tam);
		}
		else
		{
			CV::polygon(x, y, tam);
		}
	}
};


#endif // !POLYGON_SHAPE

