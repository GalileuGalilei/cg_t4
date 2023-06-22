#include "PolygonShape.h"
#include "gl_canvas2d.h"

//funções auxiliares
bool OnLine(Vector2 p1, Vector2 p2, Vector2 p3)
{
	if
		(
			p2.x <= std::max(p1.x, p3.x) &&
			p2.x >= std::min(p1.x, p3.x) &&
			p2.y <= std::max(p1.y, p3.y) &&
			p2.y >= std::min(p1.y, p3.y)
			)
	{
		return true;
	}
	return false;
}

int PolygonShape::Orientation(Vector2 p1, Vector2 p2, Vector2 p3)
{
	int val = (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y);

	if (val == 0)
	{
		return 0;  // colinear
	}

	return (val > 0) ? 1 : -1; // sentido horário ou anti-horário
}

bool PolygonShape::IsLineIntersecting(Vector2 p1, Vector2 p2, Vector2 q1, Vector2 q2)
{
	int o1 = Orientation(p1, p2, q1);
	int o2 = Orientation(p1, p2, q2);
	int o3 = Orientation(q1, q2, p1);
	int o4 = Orientation(q1, q2, p2);

	if (o1 != o2 && o3 != o4)
	{
		return true;
	}

	if (o1 == 0 && OnLine(p1, q1, p2)) return true;
	if (o2 == 0 && OnLine(p1, q2, p2)) return true;
	if (o3 == 0 && OnLine(q1, p1, q2)) return true;
	if (o4 == 0 && OnLine(q1, p2, q2)) return true;

	return false;
}



//funções da classe

PolygonShape::PolygonShape(std::vector<Vector2> points)
{
	tam = points.size();

	for (int i = 0; i < tam; i++)
	{
		this->points[0].push_back(points[i].x);
		this->points[1].push_back(points[i].y);
	}
}

PolygonShape::PolygonShape(Vector2 position, Vector2 size)
{
	Vector2 ab = position;
	Vector2 ba = position + size;
	
	points[0].push_back(ab.x);
	points[0].push_back(ab.x);
	points[0].push_back(ba.x);
	points[0].push_back(ba.x);

	points[1].push_back(ab.y);
	points[1].push_back(ba.y);
	points[1].push_back(ba.y);
	points[1].push_back(ab.y);

	tam = 4;
}

void PolygonShape::LoadData(PolygonData data, std::vector<float> x, std::vector<float> y)
{
	this->color = data.color;
	this->isFilled = data.isFilled;
	this->isCenterCalculated = data.isCenterCalculated;
	this->tam = data.tam;
	this->points[0] = x;
	this->points[1] = y;

	CalculateCenter();
}

void PolygonShape::Translate(Vector2 vector)
{
	for (int i = 0; i < tam; i++)
	{
		points[0][i] += vector.x;
		points[1][i] += vector.y;
	}

	center += vector;
}

Vector2 PolygonShape::GetCenter()
{
	if (!isCenterCalculated)
	{
		CalculateCenter();
	}
	return center;
}

void PolygonShape::SetPosition(Vector2 pos)
{
	if (!isCenterCalculated)
	{
		CalculateCenter();
	}
	Vector2 t = pos - center;
	this->Translate(t);
}

void PolygonShape::Scale(Vector2 scale, Vector2 center)
{
	if (!isCenterCalculated)
	{
		CalculateCenter();
	}
	for (int i = 0; i < tam; i++)
	{
		points[0][i] = center.x + (points[0][i] - center.x) * scale.x;
		points[1][i] = center.y + (points[1][i] - center.y) * scale.y;
	}
}

void PolygonShape::Fill()
{
	isFilled = true;
}

void PolygonShape::SetColor(Color color)
{
	this->color = color;
}

void PolygonShape::SetColor(float r, float g, float b)
{
	this->color = Color(r, g, b);
}

int PolygonShape::LineIntersectingCount(Vector2 p1, Vector2 p2, std::vector<bool>* ignoreIndex)
{
	if (tam < 2)
	{
		return 0;
	}

	int count = 0, i = 0;

	do
	{
		int next = (i + 1) % tam;
		if (ignoreIndex != NULL)
		{
			if ((*ignoreIndex)[i])
			{
				i++;
				continue;
			}
			while ((*ignoreIndex)[next])
			{
				next = (next + 1) % tam;
			}
		}
		Vector2 q1 = Vector2(points[0][i], points[1][i]);
		Vector2 q2 = Vector2(points[0][next], points[1][next]);
		if (IsLineIntersecting(p1, p2, q1, q2))
		{
			count++;
		}

		if (i > next)
		{
			break;
		}
		i = next;
	}
	while (i != 0);
	return count;
}

bool PolygonShape::PointToPolygon(Vector2 point, std::vector<bool>* ignoreIndex)
{
	if (tam < 3)
	{
		return false;
	}

	Vector2 horizontal = Vector2(100000, point.y); //linha que vai de um ponto muito distante no eixo x até o ponto
	int count = LineIntersectingCount(point, horizontal, ignoreIndex);

	return count % 2 == 1;
}

bool PolygonShape::LineToPolygon(Vector2 p1, Vector2 p2, std::vector<bool>* ignoreIndex)
{
	if (tam < 3)
	{
		return false;
	}

	int i = 0;

	do
	{
		int next = (i + 1) % tam;

		if (ignoreIndex != NULL)
		{
			if ((*ignoreIndex)[i])
			{
				i++;
				continue;
			}
			while ((*ignoreIndex)[next])
			{
				next = (next + 1) % tam;
			}
		}
		
		Vector2 q1 = Vector2(points[0][i], points[1][i]);
		Vector2 q2 = Vector2(points[0][next], points[1][next]);

		if (IsLineIntersecting(p1, p2, q1, q2))
		{
			if (Orientation(p1, p2, q1) == 0 || Orientation(p1, p2, q2) == 0)
			{
				return true;
			}

			return false;
		}

		if (i > next)
		{
			break;
		}

		i = next;
	} while (i != 0);

	return PointToPolygon(p1, ignoreIndex) && PointToPolygon(p2, ignoreIndex);
}

void PolygonShape::CalculateCenter()
{
	float x = 0, y = 0;
	for (int i = 0; i < tam; i++)
	{
		x += points[0][i];
		y += points[1][i];
	}
	center = Vector2(x / tam, y / tam);
	isCenterCalculated = true;
}

void PolygonShape::Rotate(float angle)
{
	float x, y;

	if (!isCenterCalculated)
	{
		CalculateCenter();
	}

	for (int i = 0; i < tam; i++)
	{
		x = points[0][i] - center.x;
		y = points[1][i] - center.y;
		points[0][i] = x * cos(angle) - y * sin(angle) + center.x;
		points[1][i] = x * sin(angle) + y * cos(angle) + center.y;
	}
}

void PolygonShape::Rotate(float angle, Vector2 center)
{
	float x, y;
	for (int i = 0; i < tam; i++)
	{
		x = points[0][i] - center.x;
		y = points[1][i] - center.y;
		points[0][i] = x * cos(angle) - y * sin(angle) + center.x;
		points[1][i] = x * sin(angle) + y * cos(angle) + center.y;
	}
}

void PolygonShape::Scale(Vector2 scale)
{
	float x, y;
	
	if (!isCenterCalculated)
	{
		CalculateCenter();
	}

	for (int i = 0; i < tam; i++)
	{
		x = points[0][i] - center.x;
		y = points[1][i] - center.y;
		points[0][i] = x * scale.x + center.x;
		points[1][i] = y * scale.y + center.y;
	}
}

void PolygonShape::SetRender(bool state)
{
	isRenderEnabled = state;
}