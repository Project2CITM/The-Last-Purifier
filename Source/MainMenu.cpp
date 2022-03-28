#include "MainMenu.h"
#include "RenderObject.hpp"
#include "ModuleRender.h"
#include "ModuleTextures.h"

MainMenu::MainMenu():Scene("MainMenu")
{

}

MainMenu::~MainMenu()
{
}

bool MainMenu::InitScene()
{

	Scene::InitScene();

	return true;
}

bool MainMenu::Start()
{
	fondo_ = app->textures->Load("Assets/Sprites/UI/MainMenu/mainmenu.png");

	fondo.InitAsTexture(fondo_, { 0,0 }, {0,0,0,0}, 0.5f);
	Scene::Start();

	return true;
}

bool MainMenu::PreUpdate()
{
	Scene::PreUpdate();

	return true;
}

bool MainMenu::Update()
{
	Scene::Update();

	return true;
}

bool MainMenu::PostUpdate()
{
	app->renderer->AddRenderObjectRenderQueue(fondo);
	//app->renderer->AddTextureRenderQueue(fondo_, { 0,0 }, { 0,0,1280,780 });
	Scene::PostUpdate();

	return true;
}

bool MainMenu::CleanUp()
{
	Scene::CleanUp();

	return true;
}