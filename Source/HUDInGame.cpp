#include "HUDInGame.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

HUDInGame::HUDInGame() :Scene("HUDInGame")
{

}

HUDInGame::~HUDInGame()
{
}

bool HUDInGame::InitScene()
{

	Scene::InitScene();

	return true;
}

bool HUDInGame::Start()
{

	Scene::Start();

	return true;
}

bool HUDInGame::PreUpdate()
{

	Scene::PreUpdate();

	return true;
}

bool HUDInGame::Update()
{

	Scene::Update();

	return true;
}

bool HUDInGame::PostUpdate()
{
	app->renderer->AddRectRenderQueue({ 25, 75, 500, 300 }, { 100, 100, 100, 255 });

	Scene::PostUpdate();

	return true;
}

bool HUDInGame::CleanUp()
{
	Scene::CleanUp();

	return true;
}