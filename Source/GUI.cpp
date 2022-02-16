#include "GUI.h"
#include "Application.h"

GUI::GUI(Application* app)
{
    _app = app;
}

GUI::~GUI()
{
}

void GUI::InitAsBox(int x, int y, int w, int h)
{
    currentShape = UIShape::BOX;
    boxShape.h = h;
    boxShape.w = w;
    position.x = x;
    position.y = y;
}

void GUI::InitAsCircle(int x, int y, int radius)
{
    currentShape = UIShape::CIRCLE;
    circleShape.radius = radius;
    position.x = x;
    position.y = y;
}

void GUI::Update()
{
}

void GUI::PostUpdate()
{
}

bool GUI::CheckOnMouse()
{
    float screenOffset = _app->FullScreenDesktop ? (float)_app->renderer->displayMode.h / 640.0f : 1;
    float widthOffset = _app->FullScreenDesktop ? (_app->renderer->displayMode.w - (640.0f * screenOffset)) / 2 : 0;

    iPoint mousePos = {_app->input->GetMouseX(), _app->input->GetMouseY()};

    if (currentShape == UIShape::CIRCLE)
    {
        if (mousePos.DistanceTo(position * _app->window->scale * screenOffset) < (circleShape.radius * _app->window->scale * screenOffset))return true;
    }
    else
    {
        if (mousePos.x < (position.x * _app->window->scale * screenOffset) + widthOffset || mousePos.x > ((position.x + boxShape.w) * _app->window->scale * screenOffset) + widthOffset
            || mousePos.y < position.y * _app->window->scale * screenOffset || mousePos.y > (position.y + boxShape.h) * _app->window->scale * screenOffset) return false;
        else return true;
    }
    return false;
}