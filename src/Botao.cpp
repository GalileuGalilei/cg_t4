#include "Botao.h"


Button::Button(Vector2 position, Vector2 size, Color color, const char label[], std::function<void()> callback)
{
	this->position = position;
	this->size = size;
	this->color = color;
	pressedColor = color;
	normalColor = color;
	this->callback = callback;
	strcpy_s(this->label, label);
}

void Button::OnRender(OnRenderEvent* args)
{
	CV::color(color.r, color.g, color.b);
	CV::rectFill(position, position + size);
	CV::color(0, 0, 0);
	CV::text(position.x + 5, position.y + size.y / 2, label);
}

void Button::OnClick(OnClickEvent* args)
{
	if (args->state != 0 || !CheckBounds(args->x, args->y))
	{
		color = normalColor;
		return;
	}
	color = pressedColor;
	callback();
}

void Button::OnMouseOver(OnMouseOverEvent* args)
{
	if (!CheckBounds(args->x, args->y))
	{
		return;
	}
}

bool Button::CheckBounds(int x, int y)
{
	if (x > position.x + size.x || x < position.x)
	{
		return false;
	}
	if (y > position.y + size.y || y < position.y)
	{
		return false;
	}
	return true;
}

