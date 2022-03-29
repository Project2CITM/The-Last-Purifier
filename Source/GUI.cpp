#include "GUI.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"

GUI::GUI()
{
    app = Application::GetInstance();
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
    float screenOffset = app->FullScreenDesktop ? (float)app->renderer->displayMode.h / 720.0f : 1;
    float widthOffset = app->FullScreenDesktop ? (app->renderer->displayMode.w - (1280.0f * screenOffset)) / 2 : 0;

    iPoint mousePos = {app->input->GetMouseX(), app->input->GetMouseY()};

    if (currentShape == UIShape::CIRCLE)
    {
        if (mousePos.DistanceTo(position * app->window->scale * screenOffset) < (circleShape.radius * app->window->scale * screenOffset))return true;
    }
    else
    {
        if (mousePos.x < (position.x * app->window->scale * screenOffset) + widthOffset || mousePos.x > ((position.x + boxShape.w) * app->window->scale * screenOffset) + widthOffset
            || mousePos.y < position.y * app->window->scale * screenOffset || mousePos.y > (position.y + boxShape.h) * app->window->scale * screenOffset) return false;
        else return true;
    }
    return false;
}