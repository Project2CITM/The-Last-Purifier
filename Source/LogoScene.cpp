#include "LogoScene.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleScene.h"

RenderObject Logo;

LogoScene::LogoScene():Scene("logoScene")
{
}

LogoScene::~LogoScene()
{
}

bool LogoScene::InitScene()
{
	Scene::InitScene();

	return true;
}

bool LogoScene::Start()
{
	Logo.InitAsTexture(app->textures->Load("Assets/Sprites/UI/logoGame.png"), { 155,20 }, { 0,0,0,0 }, 0.3f);

	Scene::Start();

	return true;
}

bool LogoScene::PreUpdate()
{
	Scene::PreUpdate();

	return true;
}

bool LogoScene::Update()
{
	if (count > 255)
		app->scene->ChangeCurrentSceneRequest(MAIN_MENU);
	else
		count++;

	Scene::Update();

	return true;
}

bool LogoScene::PostUpdate()
{
	app->renderer->AddRenderObjectRenderQueue(Logo);

	Scene::PostUpdate();

	return true;
}

bool LogoScene::CleanUp()
{
	Scene::CleanUp();

	return true;
}