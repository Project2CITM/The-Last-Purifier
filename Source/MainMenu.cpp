#include "MainMenu.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "GUIButton.h"
#include "ModuleInput.h"

#include <shellapi.h>

// Si no lo pone aqui, sale un menoryleak, nose porque
RenderObject fondo;

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
	fondo.InitAsTexture(app->textures->Load("Assets/Sprites/UI/MainMenu/mainmenu.png"), { 0,0 }, {0,0,0,0}, 0.5f);

	PlayBUT = new GUIButton({ 125, 175 }, 75, 25);
	ExitBUT = new GUIButton({ 150, 285 }, 50, 20);
	LinkBUT = new GUIButton({ 20, 330 }, 75, 25);

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
	//LOG("%d", PlayBUT->navigation);

	if (PlayBUT->navigation)
	{
		app->renderer->AddRenderObjectRenderQueue(fondo);
	}
	else
	{
		app->renderer->ClearRederQueue();
	}

	if (PlayBUT->doAction)
	{
		app->scene->ChangeCurrentSceneRequest(LEVEL_1);
	}



	if (ExitBUT->doAction)
	{
		return false;
	}

	if (app->input->GetMouseButton(1) == KEY_DOWN)
	{
		link = true;
	}

	if (LinkBUT->doAction && link)
	{
		ShellExecuteA(NULL, "open", "https://www.google.com", NULL, NULL, SW_SHOWNORMAL);		//change the url for the url of the web
		link = false;
	}

	Scene::Update();

	return true;
}

bool MainMenu::PostUpdate()
{
	app->renderer->AddRenderObjectRenderQueue(fondo);

	Scene::PostUpdate();

	return true;
}

bool MainMenu::CleanUp()
{
	Scene::CleanUp();

	return true;
}