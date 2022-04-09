#include "GUI.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "Scene.h"

GUI::GUI()
{
    app = Application::GetInstance();
}

GUI::~GUI()
{
}

void GUI::InitAsBox(int x, int y, int w, int h, MenuButton currentMenu)
{
    currentShape = UIShape::BOX;
    boxShape.h = h;
    boxShape.w = w;
    position.x = x;
    position.y = y;

    if(currentMenu == MenuButton::MAIN)
        app->scene->scenes[app->scene->currentScene]->AddGUIMainMenu(this);

    if(currentMenu == MenuButton::OPTIONS)
        app->scene->scenes[app->scene->currentScene]->AddGUIOptions(this);

    if(currentMenu == MenuButton::CREDITS)
        app->scene->scenes[app->scene->currentScene]->AddGUICredtis(this);

    if(currentMenu == MenuButton::INGAMEPUASE)
        app->scene->scenes[app->scene->currentScene]->AddGUIPause(this);

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
    float widthOffset = app->FullScreenDesktop ? (app->renderer->displayMode.w - (1280.0f * screenOffset)) / 2 : 1;

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