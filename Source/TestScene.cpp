#include "TestScene.h"

TestScene::TestScene(Application* app):Scene(app,"testScene")
{
}

TestScene::~TestScene()
{
}

bool TestScene::Start()
{
    return false;
}

bool TestScene::PreUpdate()
{
    rm.PreUpdate();
}

bool TestScene::Update()
{
    rm.Update();
}

bool TestScene::PostUpdate()
{
    rm.PostUpdate();
}
