#pragma once
#include "gl_canvas2d.h"
#include "GameEvents.h"
#include <iostream>
#include <string>
#include <functional>

/// <summary>
/// Classe que desenha um slider horizontal na tela que pode ser ajustado com o mouse
/// </summary>
class Slider : IClickable, IRenderable
{
private:
    float x, y, r;
    float width;
    float value;
    float maxValue;
    float minValue;
    bool isDragging;
    const char* label;
    std::function<int(int)> OnChange;

    const float thickness = 5;

public:
    Slider(std::function<int(int)> OnChange, Vector2 position, Vector2 size, Vector2 valueRange, const char* label)
    {
        this->x = position.x;
        this->y = position.y;
        this->r = size.y / 4;
        this->width = size.x;
        this->minValue = valueRange.x;
        this->maxValue = valueRange.y;
        this->label = label;
        this->OnChange = OnChange;

        this->value = minValue;
        this->isDragging = false;
    }

    float GetValue()
    {
        return value;
    }

    void OnRender(OnRenderEvent* args) override
    {
        CV::color(0.2, 0.2, 0.2);
        CV::text(x, y + 20, label);
        CV::rectFill(x, y, x + width, y + thickness);

        float normValue = (value - minValue) / maxValue;
        float cx = x + normValue * width;

        //valor atual
        CV::color(0,0,0);
        CV::text(cx + 2, y - 7, std::to_string((int)value).c_str());
        CV::color(0.8, 0.6, 0.2);
        CV::circleFill(cx, y + thickness / 2, r, 12);
    }

    void OnClick(OnClickEvent* args) override
    {
        float normValue = (value - minValue) / maxValue;
        float cx = x + normValue * width;
        // Verifica se o botão do mouse foi pressionado
        if (args->button == 0 && args->state == 0)
        {
            float dist = std::sqrt(std::pow(args->x - cx, 2) + std::pow(args->y - y - thickness / 2, 2));
            if (dist <= r)
            {
                isDragging = true;
            }
        }

        // Verifica se o botão do mouse foi solto
        else if (args->button == 0 && args->state == 1)
        {
            isDragging = false;
        }
    }

    void OnMouseOver(OnMouseOverEvent* args) override
    {
        float ratio = width / (maxValue - minValue);

        // Se estiver arrastando o círculo
        if (isDragging)
        {
            value = args->x - x;
            value /= ratio;
            value = std::max(value, minValue);
            value = std::min(value, maxValue);
            value = OnChange(value);
        }
    }

    GameLayer GetLayer() override
    {
        return GameLayer::UI;
    }
};


