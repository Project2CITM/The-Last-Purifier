#include "MainMenu.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "GUIButton.h"
#include "GUISlider.h"
#include "GUICheckbox.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleAudio.h"

#include <shellapi.h>

// Si no lo pone aqui, sale un menoryleak, nose porque
RenderObject fondo;
RenderObject options;
RenderObject credtis;

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
	fondo.InitAsTexture(app->textures->Load("Assets/Sprites/UI/background.png"), { 0,0 }, {0,0,0,0}, 0.5f);
	options.InitAsTexture(app->textures->Load("Assets/Sprites/UI/Options.png"), { 0,0 }, {0,0,0,0}, 0.5f);
	credtis.InitAsTexture(app->textures->Load("Assets/Sprites/UI/CredtisCre.png"), { 0,0 }, {0,0,0,0}, 0.5f);

	//background del menu
	app->audio->PlayMusic("Assets/Audio/Ambience/amb_dungeon1_1.ogg");	//Poner la musica que toca

		/*TODO*/
	//menu appearing
	Appear_FX = app->audio->LoadFx("Assets/Audio/SFX/UI/LogoFX.wav");	//Buscar audio apriopiado
	app->audio->PlayFx(Appear_FX);


	PlayBUT = new GUIButton({ 125, 180 }, 75, 28, MenuButton::MAIN, "Assets/Sprites/UI/PlayBUT.png");
	OptionsBUT = new GUIButton({ 140, 215 }, 60, 16, MenuButton::MAIN, "Assets/Sprites/UI/OptionBUT.png"); 
	CreditBUT = new GUIButton({ 140, 240 }, 60, 16, MenuButton::MAIN, "Assets/Sprites/UI/CredtisBUT.png"); 
	ExitBUT = new GUIButton({ 150, 285 }, 50, 15, MenuButton::MAIN, "Assets/Sprites/UI/QuitBUT.png");
	LinkBUT = new GUIButton({ 20, 330 }, 59, 16, MenuButton::MAIN, "Assets/Sprites/UI/linkBUT.png"); 

	CloseOptBUT = new GUIButton({ 297, 300 }, 46, 46, MenuButton::OPTIONS, "Assets/Sprites/UI/Back.png");

	MusicSlider = new GUISlider({ 200, 125 }, 300, 14, MenuButton::OPTIONS, "Assets/Sprites/UI/Slider1.png");
	MusicBUT = new GUIButton({ 200, 125 }, 27, 46, MenuButton::OPTIONS, "Assets/Sprites/UI/fireSlider.png");
	MusicSlider->CreateGUIBtn(MusicBUT);

	fxSlider = new GUISlider({ 200, 200 }, 300, 14, MenuButton::OPTIONS, "Assets/Sprites/UI/Slider1.png");
	fxBUT = new GUIButton({ 200, 200 }, 27, 46, MenuButton::OPTIONS, "Assets/Sprites/UI/fireSlider.png");
	fxSlider->CreateGUIBtn(fxBUT);

	CredtisCre = new GUIButton({ 125, 70 }, 117, 47, MenuButton::CREDITS, "Assets/Sprites/UI/Credtis1.png");
	CredtisAud = new GUIButton({ 125, 130 }, 117, 47, MenuButton::CREDITS, "Assets/Sprites/UI/Credtis2.png");
	CredtisArt = new GUIButton({ 125, 190 }, 117, 47, MenuButton::CREDITS, "Assets/Sprites/UI/Credtis3.png");
	CredtisOtr = new GUIButton({ 125, 250 }, 117, 47, MenuButton::CREDITS, "Assets/Sprites/UI/Credtis4.png");

	FullScreenCHK = new GUICheckbox({ 350, 215 }, 60, 60, MenuButton::OPTIONS, "Assets/Sprites/UI/CheckBox.png");

	CloseCrdBUT = new GUIButton({ 297, 300 }, 46, 46, MenuButton::CREDITS, "Assets/Sprites/UI/Back.png");

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
			ShellExecuteA(NULL, "open", "https://github.com/Project2CITM/The-last-purifier/wiki", NULL, NULL, SW_SHOWNORMAL);		//change the url for the url of the web
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

		if (FullScreenCHK->isActive)
		{
			app->fullScreen = true;
			FullScreenCHK->doAction;
		}
		else
		{
			FullScreenCHK->doAction;
			app->fullScreen = false;
		}

		if (FullScreenCHK->doAction)
		{
			app->window->ToggleFullScreen(app->fullScreen);
			FullScreenCHK->doAction = false;
		}

		app->musicVol = MusicSlider->GetValue() * 255;
		app->fxVol = fxSlider->GetValue() * 255;
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

		app->renderer->AddRenderObjectRenderQueue(options);
		//app->renderer->AddRectRenderQueue({ 100, 25, 500, 300 }, { 140, 215, 0, 255 });
	}

	if (currentMenu == CurrentMenu::Credtis)
	{
		for (int i = 0; i < guisCredtis.count(); i++)
		{
			if (guisCredtis[i]) guisCredtis[i]->PostUpdate();
		}
		app->renderer->AddRenderObjectRenderQueue(credtis);
	}

	if (currentMenu == CurrentMenu::Main)
	{
		for (int i = 0; i < guisMainMenu.count(); i++)
		{
			if (guisMainMenu[i]) guisMainMenu[i]->PostUpdate();
		}
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