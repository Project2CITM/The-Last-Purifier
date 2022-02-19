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

    return true;
}

bool TestScene::Update()
{
    roomManager.Update();

    return true;
}

bool TestScene::PostUpdate()
{
    roomManager.PostUpdate();

    return true;
}

bool TestScene::CleanUp()
{
    roomManager.CleanUp();

    return false;
}
