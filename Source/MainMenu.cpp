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
RenderObject credtis1;
RenderObject credtis2;

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
	credtis1.InitAsTexture(app->textures->Load("Assets/Sprites/UI/CredtisCre.png"), { 0,0 }, {0,0,0,0}, 0.5f);
	credtis2.InitAsTexture(app->textures->Load("Assets/Sprites/UI/CreditsProba.png"), { 0,0 }, {0,0,0,0}, 0.5f);

	//background del menu
	app->audio->PlayMusic("Assets/Audio/Ambience/amb_dungeon1_1.ogg");	//Poner la musica que toca

		/*TODO*/
	//menu appearing
	Appear_FX = app->audio->LoadFx("Assets/Audio/SFX/UI/LogoFX.wav");	//Buscar audio apriopiado
	app->audio->PlayFx(Appear_FX);


	PlayBUT = new GUIButton({ 125, 180 }, 75, 28, MenuButton::MAIN, "Assets/Sprites/UI/PlayBUT.png");
	OptionsBUT = new GUIButton({ 132, 215 }, 60, 16, MenuButton::MAIN, "Assets/Sprites/UI/OptionBUT.png");
	CreditBUT = new GUIButton({ 132, 240 }, 60, 16, MenuButton::MAIN, "Assets/Sprites/UI/CredtisBUT.png");
	ExitBUT = new GUIButton({ 132, 285 }, 50, 15, MenuButton::MAIN, "Assets/Sprites/UI/QuitBUT.png");
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
	int leftYMain, leftYOptions, leftXOptions, leftYCredtis;
	leftYMain = app->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTY);
	leftYOptions = app->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTY);
	leftXOptions = app->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTX);
	leftYCredtis = app->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTY);

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
		if (app->input->usingGameController)
		{
			if (leftYMain > 0 && !AxisPress && ControllerPos <= 3)
			{
				ControllerPos += 1;
				AxisPress = true;
			}
			else if (leftYMain < 0 && !AxisPress && ControllerPos >= 1)
			{
				ControllerPos -= 1;
				AxisPress = true;
			}
			else if (leftYMain == 0)
			{
				AxisPress = false;
			}
		}
		LOG("%d", ControllerPos);

		
		if (PlayBUT->doAction || (ControllerPos == 0 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN))
		{
			app->scene->ChangeCurrentSceneRequest(LEVEL_1);
		}

		if (OptionsBUT->doAction || (ControllerPos == 1 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN))
		{
			currentMenu = CurrentMenu::Options;
			OptionsBUT->doAction = false;
		}

		if (CreditBUT->doAction || (ControllerPos == 2 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN))
		{
			currentMenu = CurrentMenu::Credtis;
			CreditBUT->doAction = false;
		}

		if (ExitBUT->doAction || (ControllerPos == 3 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN))
		{
			return false;
		}

		if (LinkBUT->doAction || (ControllerPos == 4 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN))
		{
			ShellExecuteA(NULL, "open", "https://github.com/Project2CITM/The-last-purifier/wiki", NULL, NULL, SW_SHOWNORMAL);		//change the url for the url of the web
			LinkBUT->doAction = false;
		}
	}

	if (currentMenu == CurrentMenu::Options)
	{
		if (app->input->usingGameController)
		{
			if (leftYOptions > 0 && !AxisPress && ControllerPosOpY <= 2)
			{
				ControllerPosOpY += 1;
				AxisPress = true;
			}
			else if (leftYOptions < 0 && !AxisPress && ControllerPosOpY >= 1)
			{
				ControllerPosOpY -= 1;
				AxisPress = true;
			}
			else if (leftYOptions == 0)
			{
				AxisPress = false;
			}
		}
		LOG("%d", fxBUTpos);

		if (CloseOptBUT->doAction || (ControllerPosOpY == 3 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN) || app->input->GetControllerButton(BUTTON_B) == KEY_DOWN)
		{
			currentMenu = CurrentMenu::Main;
			ControllerPosOpY = 0;
			CloseOptBUT->doAction = false;
		}

		if (FullScreenCHK->isActive || (ControllerPosOpY == 2 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN))
		{
			app->fullScreen = true;
			FullScreenCHK->doAction;
		}
		else
		{
			FullScreenCHK->doAction;
			app->fullScreen = false;
		}

		if (FullScreenCHK->doAction || (ControllerPosOpY == 2 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN))
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

		if (CredtisCre->doAction)
		{
			currentCredtis = CurrentCredtis::Creadors;
			CredtisCre->doAction = false;
		}

		if (CredtisAud->doAction)
		{
			currentCredtis = CurrentCredtis::Audio;
			CredtisAud->doAction = false;
		}

		if (CredtisArt->doAction)
		{
			currentCredtis = CurrentCredtis::Art;
			CredtisArt->doAction = false;
		}

		if (CredtisOtr->doAction)
		{
			currentCredtis = CurrentCredtis::Otros;
			CredtisOtr->doAction = false;
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

		if(currentCredtis == CurrentCredtis::Creadors)		app->renderer->AddRenderObjectRenderQueue(credtis1);

		if(currentCredtis == CurrentCredtis::Audio)		app->renderer->AddRenderObjectRenderQueue(credtis2);

		if(currentCredtis == CurrentCredtis::Art)		app->renderer->AddRenderObjectRenderQueue(credtis2);

		if(currentCredtis == CurrentCredtis::Otros)		app->renderer->AddRenderObjectRenderQueue(credtis2);
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