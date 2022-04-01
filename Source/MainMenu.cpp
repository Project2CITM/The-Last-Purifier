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

	PlayBUT = new GUIButton({ 125, 180 }, 75, 25, "Assets/Sprites/UI/PlayButton/playB.png");
	OptionsBUT = new GUIButton({ 140, 215 }, 60, 20);
	CreditBUT = new GUIButton({ 140, 240 }, 60, 20);
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

	if (PlayBUT->doAction)
	{
		app->scene->ChangeCurrentSceneRequest(LEVEL_1);
	}

	if (OptionsBUT->doAction)
	{
		OptionsMenu = true;
		OptionsBUT->doAction = false;
	}

	if (CreditBUT->doAction)
	{
		CreditsMenu = false;
		CreditBUT->doAction = false;
	}

	if (ExitBUT->doAction)
	{
		return false;
	}

	if (LinkBUT->doAction)
	{
		ShellExecuteA(NULL, "open", "https://www.google.com", NULL, NULL, SW_SHOWNORMAL);		//change the url for the url of the web
		LinkBUT->doAction = false;
	}

	Scene::Update();

	return true;
}

bool MainMenu::PostUpdate()
{
	if (OptionsMenu)
	{
		app->renderer->AddRenderObjectRenderQueue(fondo);
	}

	Scene::PostUpdate();

	return true;
}

bool MainMenu::CleanUp()
{
	Scene::CleanUp();

	return true;
}