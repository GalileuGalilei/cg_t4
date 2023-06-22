#ifndef __BOTAO_H__
#define __BOTAO_H__

#include <functional>
#include <string.h>
#include "GameEvents.h"
#include "gl_canvas2d.h"

class Button : IRenderable, IClickable
{
private:
	std::function<void()> callback;

protected:

	const float offset = 0.2f;
	char label[30];
	bool isMouseOver = false;
	Color color;
	Color normalColor;
	Color pressedColor;
	Vector2 position, size;

public:
	Button(Vector2 position, Vector2 size, Color color, const char label[], std::function<void()> callback);

protected:
	void OnRender(OnRenderEvent* args) override;

	void OnClick(OnClickEvent* args) override;

	void OnMouseOver(OnMouseOverEvent* args) override;

	bool CheckBounds(int x, int y);
};

#endif
