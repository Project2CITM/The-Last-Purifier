#include "MainMenu.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "GUIButton.h"
#include "GUISlider.h"
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

//1280 * 720
//640 * 360

bool MainMenu::Start()
{
	fondo.InitAsTexture(app->textures->Load("Assets/Sprites/UI/MainMenu/mainmenu.png"), { 0,0 }, {0,0,0,0}, 0.5f);

	PlayBUT = new GUIButton({ 125, 180 }, 75, 25, MenuButton::MAIN, "Assets/Sprites/UI/PlayButton/playB.png");
	OptionsBUT = new GUIButton({ 140, 215 }, 60, 20, MenuButton::MAIN);
	CreditBUT = new GUIButton({ 140, 240 }, 60, 20, MenuButton::MAIN);
	ExitBUT = new GUIButton({ 150, 285 }, 50, 20, MenuButton::MAIN);
	LinkBUT = new GUIButton({ 20, 330 }, 75, 25, MenuButton::MAIN);

	CloseOptBUT = new GUIButton({ 285, 330 }, 75, 25, MenuButton::OPTIONS);
	MusicSlider = new GUISlider({ 10, 10 }, 100, 25, MenuButton::OPTIONS);

	CloseCrdBUT = new GUIButton({ 285, 330 }, 75, 25, MenuButton::CREDITS);

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
	if (currentMenu == CurrentMenu::Main)
	{
		for (int i = 0; i < guisMainMenu.count(); i++)
		{
			if (guisMainMenu[i]) guisMainMenu[i]->Update();
		}
	}

	if (currentMenu == CurrentMenu::Options)
	{
		for (int i = 0; i < guisOptions.count(); i++)
		{
			if (guisOptions[i]) guisOptions[i]->Update();
		}
	}

	if (currentMenu == CurrentMenu::Credtis)
	{
		for (int i = 0; i < guisCredtis.count(); i++)
		{
			if (guisCredtis[i]) guisCredtis[i]->Update();
		}
	}

	if(currentMenu == CurrentMenu::Main)
	{
		if (PlayBUT->doAction)
		{
			app->scene->ChangeCurrentSceneRequest(LEVEL_1);
		}

		if (OptionsBUT->doAction)
		{
			currentMenu = CurrentMenu::Options;
			OptionsBUT->doAction = false;
		}

		if (CreditBUT->doAction)
		{
			currentMenu = CurrentMenu::Credtis;
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
	}

	if (currentMenu == CurrentMenu::Options)
	{
		if (CloseOptBUT->doAction)
		{
			currentMenu = CurrentMenu::Main;
			CloseOptBUT->doAction = false;
		}
	}

	if (currentMenu == CurrentMenu::Credtis)
	{
		if (CloseCrdBUT->doAction)
		{
			currentMenu = CurrentMenu::Main;
			CloseCrdBUT->doAction = false;
		}
	}

	Scene::Update();

	return true;
}

bool MainMenu::PostUpdate()
{
	if (currentMenu == CurrentMenu::Options)
	{
		for (int i = 0; i < guisOptions.count(); i++)
		{
			if (guisOptions[i]) guisOptions[i]->PostUpdate();
		}

		app->renderer->AddRectRenderQueue({ 100, 25, 500, 300 }, { 140, 215, 0, 255 });
	}

	if (currentMenu == CurrentMenu::Credtis)
	{
		for (int i = 0; i < guisCredtis.count(); i++)
		{
			if (guisCredtis[i]) guisCredtis[i]->PostUpdate();
		}
	}

	if (currentMenu == CurrentMenu::Main)
	{
		for (int i = 0; i < guisMainMenu.count(); i++)
		{
			if (guisMainMenu[i]) guisMainMenu[i]->PostUpdate();
		}
	}

	///app->renderer->AddRenderObjectRenderQueue(fondo);


	Scene::PostUpdate();

	return true;
}

bool MainMenu::CleanUp()
{
	Scene::CleanUp();

	return true;
}