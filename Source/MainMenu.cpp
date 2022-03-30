#include "MainMenu.h"
#include "RenderObject.hpp"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "GUIButton.h"
#include "GUI.h"
#include "ModuleScene.h"

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

	PlayBUT = new GUIButton({ 0, 0 }, 100, 100);
	guis.add(PlayBUT);
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

	if (PlayBUT->navigation)
	{
		app->renderer->AddRenderObjectRenderQueue(fondo);
	}

	if (PlayBUT->doAction)
	{
		app->scene->ChangeCurrentSceneRequest(LEVEL_1);
	}
	Scene::Update();

	return true;
}

bool MainMenu::PostUpdate()
{
	//app->renderer->AddRenderObjectRenderQueue(fondo);
	Scene::PostUpdate();

	return true;
}

bool MainMenu::CleanUp()
{
	Scene::CleanUp();

	return true;
}