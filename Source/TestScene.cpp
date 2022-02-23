#include "TestScene.h"

TestScene::TestScene(Application* app):Scene(app,"testScene")
{
}

TestScene::~TestScene()
{
}

bool TestScene::Start()
{
    roomManager.Start();

    return true;
}

bool TestScene::PreUpdate()
{
    if (_app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
        roomManager.CleanUp();
        roomManager.Start();
    }

    return true;
}

bool TestScene::Update()
{
    roomManager.Update();

    return true;
}

bool TestScene::PostUpdate()
{
    // Test draw circle
    // _app->renderer->AddCircleRenderQueue(iPoint{ 50, 50 }, 40, SDL_Color{ 50,0,0,255 }, 2, 50);

    // Test draw line  
    //_app->renderer->AddLineRenderQueue(iPoint{ 50,50 }, iPoint{ 100,50 }, true, SDL_Color{ 50,0,0,255 }, 2, 50);

    roomManager.PostUpdate();
    //_app->physics->ShapesRender();
   
    return true;
}

bool TestScene::CleanUp()
{
    roomManager.CleanUp();

    return false;
}
