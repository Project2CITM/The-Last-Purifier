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
#include "AssetsManager.h"

#include <shellapi.h>

// Si no lo pone aqui, sale un menoryleak, nose porque
RenderObject fondo;
RenderObject titulo;
RenderObject options;
RenderObject credtis1;

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
	char* buffer = 0;
	pugi::xml_document dataFile;

	int bytesFile = app->assetManager->LoadData("data.xml", &buffer);

	pugi::xml_parse_result result = dataFile.load_buffer(buffer, bytesFile);

	RELEASE_ARRAY(buffer);

	fondo.InitAsTexture(app->textures->Load("Sprites/UI/background.png"), { 0,0 }, { 0,0,0,0 }, 0.5f, 0, 0, 0, SDL_FLIP_NONE, 0);
	titulo.InitAsTexture(app->textures->Load("Sprites/UI/TItle.png"), { 0,0 }, { 0,0,0,0 }, 1, 1, 0, 0, SDL_FLIP_NONE, 0);
	options.InitAsTexture(app->textures->Load("Sprites/UI/Options.png"), { 0,0 }, {0,0,0,0}, 0.5f);
	options.color.a = Alfa;
	credtis1.InitAsTexture(app->textures->Load("Sprites/UI/CredtisCre.png"), { 0,0 }, {0,0,0,0}, 0.5f);
	credtis1.color.a = Alfa;

	Hover = app->audio->LoadFx("Audio/SFX/UI/sfx_uiHover.wav");
	Press = app->audio->LoadFx("Audio/SFX/UI/sfx_uiSelect.wav");

	//background del menu
	app->audio->PlayMusic("Audio/Ambience/amb_dungeon1_1.ogg", 2.0f, false);	//Poner la musica que toca

	//menu appearing
	Appear_FX = app->audio->LoadFx("Audio/SFX/UI/menuFX.wav");	//Buscar audio apriopiado
	app->audio->PlayFx(Appear_FX);


	PlayBUT = new GUIButton({ 125, 180 }, 75, 28, MenuButton::MAIN, app->textures->Load("Sprites/UI/PlayBUT.png"));
	OptionsBUT = new GUIButton({ 132, 215 }, 60, 16, MenuButton::MAIN, app->textures->Load("Sprites/UI/OptionBUT.png"));
	CreditBUT = new GUIButton({ 132, 240 }, 60, 16, MenuButton::MAIN, app->textures->Load("Sprites/UI/CredtisBUT.png"));
	ExitBUT = new GUIButton({ 132, 285 }, 50, 15, MenuButton::MAIN, app->textures->Load("Sprites/UI/QuitBUT.png"));
	LinkBUT = new GUIButton({ 20, 330 }, 59, 16, MenuButton::MAIN, app->textures->Load("Sprites/UI/linkBUT.png"));

	CloseOptBUT = new GUIButton({ 297, 300 }, 46, 46, MenuButton::OPTIONS, app->textures->Load("Sprites/UI/Back.png"));

	MusicSlider = new GUISlider({ 200, 125 }, 300, 14, MenuButton::OPTIONS, app->textures->Load("Sprites/UI/Slider1.png"));
	MusicBUT = new GUIButton({ 200, 125 }, 27, 46, MenuButton::OPTIONS, app->textures->Load("Sprites/UI/fireSlider.png"));
	MusicSlider->CreateGUIBtn(MusicBUT);
	MusicSlider->SetValue(app->audio->musicVol / 255);

	fxSlider = new GUISlider({ 200, 175 }, 300, 14, MenuButton::OPTIONS, app->textures->Load("Sprites/UI/Slider1.png"));
	fxBUT = new GUIButton({ 200, 175 }, 27, 46, MenuButton::OPTIONS, app->textures->Load("Sprites/UI/fireSlider.png"));
	fxSlider->CreateGUIBtn(fxBUT);
	fxSlider->SetValue(app->audio->fxVol / 255);

	CredtisCre = new GUIButton({ 125, 160 }, 117, 47, MenuButton::CREDITS, app->textures->Load("Sprites/UI/Credtis1.png"));
	//CredtisAud = new GUIButton({ 125, 130 }, 117, 47, MenuButton::CREDITS, app->textures->Load("Sprites/UI/Credtis2.png"));
	//CredtisArt = new GUIButton({ 125, 190 }, 117, 47, MenuButton::CREDITS, app->textures->Load("Sprites/UI/Credtis3.png"));
	//CredtisOtr = new GUIButton({ 125, 250 }, 117, 47, MenuButton::CREDITS, app->textures->Load("Sprites/UI/Credtis4.png"));

	FullScreenCHK = new GUICheckbox({ 350, 200 }, 40, 40, MenuButton::OPTIONS, app->textures->Load("Sprites/UI/CheckBox.png"));
	FullScreenCHK->ChangeState(app->FullScreenDesktop);

	VSyncCHK = new GUICheckbox({ 350, 240 }, 40, 40, MenuButton::OPTIONS, app->textures->Load("Sprites/UI/CheckBox.png"));
	VSyncCHK->ChangeState(app->vsync);

	CloseCrdBUT = new GUIButton({ 297, 300 }, 46, 46, MenuButton::CREDITS, app->textures->Load("Sprites/UI/Back.png"));

	//testTrans = app->window->width * app->window->height;

	Alfa = 0;
	AnimaTitulo = 0;
	tituloPosX = 0;
	tituloPosY = 150;
	fondoAnimAlpha = 0;

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
			if ((leftYMain > 10000 || app->input->GetControllerButton(BUTTON_DOWN) == KEY_DOWN) && !AxisPress)
			{
				ControllerPos += 1;
				if (ControllerPos > 4) ControllerPos = 0;
				AxisPress = true;
			}
			else if ((leftYMain < -10000 || app->input->GetControllerButton(BUTTON_UP) == KEY_DOWN) && !AxisPress)
			{
				ControllerPos -= 1;
				if (ControllerPos < 0) ControllerPos = 4;
				AxisPress = true;
			}
			else if (abs(leftYMain) < 2500)
			{
				AxisPress = false;
			}

			GUIButton* selectedButton = (GUIButton*)guisMainMenu.At(ControllerPos)->data;
			selectedButton->HoverButton();

			/*if (ControllerPos == 0)	app->renderer->AddRenderObjectRenderQueue(PlayController);
			if (ControllerPos == 1)	app->renderer->AddRenderObjectRenderQueue(SettingsController);
			if (ControllerPos == 2)	app->renderer->AddRenderObjectRenderQueue(CredtisController);
			if (ControllerPos == 3)	app->renderer->AddRenderObjectRenderQueue(QuitController);
			if (ControllerPos == 4)	app->renderer->AddRenderObjectRenderQueue(LinkController);*/
		}
		
		if (PlayBUT->doAction || (ControllerPos == 0 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN))
		{
			app->scene->ChangeCurrentSceneRequest(HUB);
			PlayBUT->PressButton();
		}

		if (OptionsBUT->doAction || (ControllerPos == 1 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN))
		{
			currentMenu = CurrentMenu::Options;
			PlayBUT->PressButton();
			OptionsBUT->doAction = false;
		}

		if (CreditBUT->doAction || (ControllerPos == 2 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN))
		{
			currentMenu = CurrentMenu::Credtis;
			CreditBUT->PressButton();
			CreditBUT->doAction = false;
			return true;
		}

		if (ExitBUT->doAction || (ControllerPos == 3 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN))
		{
			ExitBUT->PressButton();
			app->ExitGame();
		}

		if (LinkBUT->doAction || (ControllerPos == 4 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN))
		{
			ShellExecuteA(NULL, "open", "https://github.com/Project2CITM/The-last-purifier/wiki", NULL, NULL, SW_SHOWNORMAL);		//change the url for the url of the web
			LinkBUT->PressButton();
			LinkBUT->doAction = false;
		}
	}

	if (currentMenu == CurrentMenu::Options)
	{
		if (app->input->usingGameController)
		{
			if ((leftYOptions > 10000 || app->input->GetControllerButton(BUTTON_DOWN) == KEY_DOWN) && !AxisPress)
			{
				ControllerPosOpY += 1;
				if (ControllerPosOpY > 4) ControllerPosOpY = 0;
				AxisPress = true;
			}
			else if ((leftYOptions < -10000 || app->input->GetControllerButton(BUTTON_UP) == KEY_DOWN) && !AxisPress)
			{
				ControllerPosOpY -= 1;
				if (ControllerPosOpY < 0) ControllerPosOpY = 4;
				AxisPress = true;
			}
			else if (abs(leftYOptions) < 1000)
			{
				AxisPress = false;
			}

			switch (ControllerPosOpY)
			{
			case 0:
				MusicBUT->HoverButton();
				break;
			case 1:
				fxBUT->HoverButton();
				break;
			case 2:
				if (!FullScreenCHK->isActive) FullScreenCHK->checkboxState = CheckboxState::FOCUS;
				break;
			case 3:
				if (!VSyncCHK->isActive) VSyncCHK->checkboxState = CheckboxState::FOCUS;
				break;
			case 4:
				CloseOptBUT->HoverButton();
				break;
			}
		}
		if (app->input->usingGameController)
		{
			// Music controller with Gamepad
			if (ControllerPosOpY == 0 && (leftXOptions > 10000 || app->input->GetControllerButton(JoystickButtons::BUTTON_RIGHT)))
			{
				MusicSlider->SetValue(MusicSlider->GetValue() + 0.01f);
			}
			if (ControllerPosOpY == 0 && (leftXOptions < -10000 || app->input->GetControllerButton(JoystickButtons::BUTTON_LEFT)))
			{
				MusicSlider->SetValue(MusicSlider->GetValue() - 0.01f);
			}

			// SFX controller with Gamepad
			if (ControllerPosOpY == 1 && (leftXOptions > 10000 || app->input->GetControllerButton(JoystickButtons::BUTTON_RIGHT)))
			{
				fxSlider->SetValue(fxSlider->GetValue() + 0.01f);
			}
			if (ControllerPosOpY == 1 && (leftXOptions < -10000 || app->input->GetControllerButton(JoystickButtons::BUTTON_LEFT)))
			{
				fxSlider->SetValue(fxSlider->GetValue() - 0.01f);
			}
		}

		if (CloseOptBUT->doAction || (ControllerPosOpY == 4 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN) || app->input->GetControllerButton(BUTTON_B) == KEY_DOWN)
		{
			currentMenu = CurrentMenu::Main;
			ControllerPosOpY = 0;
			options.color.a = 0;
			CloseOptBUT->doAction = false;
		}

		if ((FullScreenCHK->isActive || (ControllerPosOpY == 2 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN)) && !app->FullScreenDesktop)
		{
			app->window->ToggleFullScreen(true);
			FullScreenCHK->ChangeState(true);
		}
		else if ((!FullScreenCHK->isActive || (ControllerPosOpY == 2 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN)) && app->FullScreenDesktop)
		{
			app->window->ToggleFullScreen(false);
			FullScreenCHK->ChangeState(false);
		}

		if ((VSyncCHK->isActive || (ControllerPosOpY == 3 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN)) && !app->vsync)
		{
			app->vsync = true;
			VSyncCHK->ChangeState(true);
		}
		else if ((!VSyncCHK->isActive || (ControllerPosOpY == 3 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN)) && app->vsync)
		{
			app->vsync = false;
			VSyncCHK->ChangeState(false);
		}
		app->audio->SetMusicVolume(MusicSlider->GetValue() * 255);
		app->audio->SetSFXVolume(fxSlider->GetValue() * 255);
	}

	if (currentMenu == CurrentMenu::Credtis)
	{
		if (app->input->usingGameController)
		{
			if ((leftYOptions > 10000 || app->input->GetControllerButton(BUTTON_DOWN) == KEY_DOWN) && !AxisPress)
			{
				ControllerPosCr += 1;
				if (ControllerPosCr > 1) ControllerPosCr = 0;
				AxisPress = true;
			}
			else if ((leftYOptions < -10000 || app->input->GetControllerButton(BUTTON_UP) == KEY_DOWN) && !AxisPress)
			{
				ControllerPosCr -= 1;
				if (ControllerPosCr < 0) ControllerPosCr = 1;
				AxisPress = true;
			}
			else if (abs(leftYOptions) < 1000)
			{
				AxisPress = false;
			}

			GUIButton* selectedButton = (GUIButton*)guisCredtis.At(ControllerPosCr)->data;
			selectedButton->HoverButton();
		}

		if (CloseCrdBUT->doAction || (ControllerPosCr == 1 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN) || app->input->GetControllerButton(BUTTON_B) == KEY_DOWN)
		{
			currentMenu = CurrentMenu::Main;
			credtis1.color.a = 0;
			CloseCrdBUT->doAction = false;
		}

		if (CredtisCre->doAction || (ControllerPosCr == 0 && app->input->GetControllerButton(BUTTON_A) == KEY_DOWN))
		{
			currentCredtis = CurrentCredtis::Creadors;
			CredtisCre->doAction = false;
		}
	}

	Scene::Update();

	return true;
}

bool MainMenu::PostUpdate()
{
	//app->textures->TestEffect();

	if (currentMenu == CurrentMenu::Options)
	{
		if (Alfa >= 250)
		{
			for (int i = 0; i < guisOptions.count(); i++)
			{
				if (guisOptions[i]) guisOptions[i]->PostUpdate();
			}
		}

		app->renderer->AddRenderObjectRenderQueue(options);
		//app->renderer->AddRectRenderQueue({ 100, 25, 500, 300 }, { 140, 215, 0, 255 });
	}

	if (currentMenu == CurrentMenu::Credtis)
	{
		if(Alfa >= 250)
		{ 
			for (int i = 0; i < guisCredtis.count(); i++)
			{
				if (guisCredtis[i]) guisCredtis[i]->PostUpdate();
			}
		}

		if(currentCredtis == CurrentCredtis::Creadors)		app->renderer->AddRenderObjectRenderQueue(credtis1);
	}

	if (currentMenu == CurrentMenu::Main )
	{
		if (AnimaTitulo >= 225)
		{
			for (int i = 0; i < guisMainMenu.count(); i++)
			{
				if (guisMainMenu[i]) guisMainMenu[i]->PostUpdate();
			}

			if (Alfa != 0)
			{
				Alfa = 0;
			}
		}
		else if (AnimaTitulo <= 250)
		{
			app->renderer->AddRenderObjectRenderQueue(titulo);
			app->renderer->AddRenderObjectRenderQueue(fondo);
			fondo.color.a = fondoAnimAlpha;

			if (tituloPosX <= 250 && AnimaTitulo <= 32)
			{
				titulo.destRect = { tituloPosX,100,0,0 };
				tituloPosX += 14;
			}

			if (AnimaTitulo >= 75 && tituloPosY != 26)
			{
				titulo.destRect = { tituloPosX,tituloPosY,0,0 };
				tituloPosX -= 3;
				tituloPosY -= 2;
				titulo.scale -= 0.008f;

				if(fondoAnimAlpha < 255)
					fondoAnimAlpha += 5;

			}
			AnimaTitulo += 2;

			if (tituloPosX == 52 && tituloPosY == 26)
			{
				titulo.destRect = { 65,26,0,0 };
				titulo.scale = 0.5f;
			}
		}
	}

	if (Alfa <= 250 && AnimaTitulo >= 225)
	{
		app->renderer->AddRenderObjectRenderQueue(titulo);
		app->renderer->AddRenderObjectRenderQueue(fondo);
	}

	if (currentMenu == CurrentMenu::Credtis)
	{
		if (Alfa <= 250)
		{
			Alfa += 4;
		}

		credtis1.color = { 255,255,255,Alfa };
	}

	if (currentMenu == CurrentMenu::Options)
	{
		if (Alfa <= 250)
		{
			Alfa += 4;
		}

		options.color = { 255,255,255,Alfa };
	}

	Scene::PostUpdate();

	return true;
}

bool MainMenu::CleanUp()
{
	Scene::CleanUp();

	return true;
}