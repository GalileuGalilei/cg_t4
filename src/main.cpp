/*********************************************************************
// Canvas2D
// Autor: Cesar Tadeu Pozzer
//        04/2021
// *******************************************************************/
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gl_canvas2d.h"
#include "EventManager.h"
#include "GameManager.h"
#include "Piston.h"

void keyboard(int key)
{
    OnKeyEvent keyEvent = OnKeyEvent(key, 0);
    EventManager::Instance()->InvokeEvent<OnKeyEvent>((BaseEvent*)&keyEvent);
}

void keyboardUp(int key)
{
    OnKeyEvent keyEvent = OnKeyEvent(key, 1);
    EventManager::Instance()->InvokeEvent<OnKeyEvent>((BaseEvent*)&keyEvent);
}

void render()
{
    CV::color(0, 0, 0);
    CV::rectFill(0, 0, 620, 620);

    OnUpdateEvent updateEvent = OnUpdateEvent();
    EventManager::Instance()->InvokeEvent<OnUpdateEvent>((BaseEvent*)&updateEvent);

    OnRenderEvent renderEvent = OnRenderEvent(0);
    EventManager::Instance()->InvokeEvent<OnRenderEvent>((BaseEvent*)&renderEvent);
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    if (state != -2)
    {
        OnClickEvent clickEvent = OnClickEvent(button, state, x, y);
        EventManager::Instance()->InvokeEvent<OnClickEvent>((BaseEvent*)&clickEvent);
    }

    OnMouseOverEvent mouseOverEvent = OnMouseOverEvent(x, y, direction);
    EventManager::Instance()->InvokeEvent<OnMouseOverEvent>((BaseEvent*)&mouseOverEvent);
}

int main(void)
{
    int screenWidth = 620, screenHeight = 620;
    GameManager::Instance()->SetScreen(Vector2(screenWidth, screenHeight));
    Piston* p = new Piston();

    CV::init(&screenWidth, &screenHeight, "3D - poggers");
    CV::run();
}
