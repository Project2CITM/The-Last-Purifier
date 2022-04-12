#include "HUDInGame.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "GUIButton.h"
#include "GUISlider.h"
#include "GUICheckbox.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleAudio.h"

RenderObject Controls1;
RenderObject Controls2;
RenderObject PauseBG;
RenderObject SettingsBG;

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
	PauseBG.InitAsTexture(app->textures->Load("Assets/Sprites/UI/PauseBG.png"), { app->renderer->camera->x, app->renderer->camera->y }, { 0,0,0,0 }, 0.5f, 4, 0);
	SettingsBG.InitAsTexture(app->textures->Load("Assets/Sprites/UI/FondoSettings.png"), { app->renderer->camera->x, app->renderer->camera->y }, { 0,0,0,0 }, 0.5f, 4, 0);
	Controls1.InitAsTexture(app->textures->Load("Assets/Sprites/UI/Controls1_2.png"), { app->renderer->camera->x, app->renderer->camera->y }, { 0,0,0,0 }, 0.5f, 4, 1);
	Controls2.InitAsTexture(app->textures->Load("Assets/Sprites/UI/Controls2_2.png"), { app->renderer->camera->x, app->renderer->camera->y }, { 0,0,0,0 }, 0.5f, 4, 1);

	hpRect = { app->renderer->camera->x + 15, app->renderer->camera->y + 10, 200, 10 };

	miniMap = { app->renderer->camera->x + 535, app->renderer->camera->y + 5, 100, 100 };

	spell1 = { app->renderer->camera->x + 225, app->renderer->camera->y + 314, 30, 40 };
	spell2 = { app->renderer->camera->x + 275, app->renderer->camera->y + 314, 30, 40 };
	spell3 = { app->renderer->camera->x + 325, app->renderer->camera->y + 314, 30, 40 };
	spell4 = { app->renderer->camera->x + 375, app->renderer->camera->y + 314, 30, 40 };

	pause = { app->renderer->camera->x, app->renderer->camera->y, 500, 300 };

	resumeBUT = { app->renderer->camera->x + 262, app->renderer->camera->y + + 70};//640 pixeles with pantalla
	settingsBUT = { app->renderer->camera->x + 262, app->renderer->camera->y + 117};
	controlsBUT = { app->renderer->camera->x + 262, app->renderer->camera->y +164};
	giveUpBUT = { app->renderer->camera->x + 262, app->renderer->camera->y +211};
	quitBUT = { app->renderer->camera->x + 262, app->renderer->camera->y +258};

	ResumeBUT = new GUIButton(resumeBUT, 117, 47, MenuButton::INGAMEPUASE, "Assets/Sprites/UI/Resume.png");
	SettingsBUT = new GUIButton(settingsBUT, 117, 47, MenuButton::INGAMEPUASE, "Assets/Sprites/UI/Settings.png");
	ControlsBUT = new GUIButton(controlsBUT, 117, 47, MenuButton::INGAMEPUASE, "Assets/Sprites/UI/Controls.png");
	GiveUpBUT = new GUIButton(giveUpBUT, 117, 47, MenuButton::INGAMEPUASE, "Assets/Sprites/UI/giveUp.png");
	QuitBUT = new GUIButton(quitBUT, 117, 47, MenuButton::INGAMEPUASE, "Assets/Sprites/UI/Quit.png");

	CloseControlsBUT = new GUIButton({ app->renderer->camera->x + 297, app->renderer->camera->y + 315 }, 46, 46, MenuButton::CONTROLSPAUSE, "Assets/Sprites/UI/Back.png");

	CloseSettingsBUT = new GUIButton({ app->renderer->camera->x + 297, app->renderer->camera->y + 315 }, 46, 46, MenuButton::SETTINGSPAUSE, "Assets/Sprites/UI/Back.png");

	MusicBUT = new GUIButton({ app->renderer->camera->x + 200, app->renderer->camera->y + 125 }, 27, 46, MenuButton::SETTINGSPAUSE, "Assets/Sprites/UI/fireSlider.png");
	MusicSlider = new GUISlider({ app->renderer->camera->x + 200, app->renderer->camera->y + 125 }, 300, 14, MenuButton::SETTINGSPAUSE, "Assets/Sprites/UI/Slider1.png");
	MusicSlider->CreateGUIBtn(MusicBUT);

	fxBUT = new GUIButton({ app->renderer->camera->x + 200, app->renderer->camera->y + 200 }, 27, 46, MenuButton::SETTINGSPAUSE, "Assets/Sprites/UI/fireSlider.png");
	fxSlider = new GUISlider({ app->renderer->camera->x + 200, app->renderer->camera->y + 200 }, 300, 14, MenuButton::SETTINGSPAUSE, "Assets/Sprites/UI/Slider1.png");
	fxSlider->CreateGUIBtn(fxBUT);

	FullScreenCHK = new GUICheckbox({ app->renderer->camera->x + 350, app->renderer->camera->y + 215 }, 60, 60, MenuButton::SETTINGSPAUSE, "Assets/Sprites/UI/CheckBox.png");


	Scene::Start();

	return true;
}

bool HUDInGame::PreUpdate()
{
	if (!app->isPause)
	{
		if (startPause)
		{
			app->musicVol = app->musicVol * 2;
			app->fxVol = app->fxVol * 2;
			//currentPauseMenu = CurrentPauseMenu::Pause;
			startPause = false;
		}
	}


	Scene::PreUpdate();

	return true;
}

bool HUDInGame::Update()
{
	if (app->isPause)
	{
		if (!startPause)
		{
			app->musicVol = app->musicVol * 0.5f ;
			app->fxVol = app->fxVol * 0.5f ;
			startPause = true;
		}

		if (currentPauseMenu == CurrentPauseMenu::Pause)
		{
			for (int i = 0; i < guisPause.count(); i++)
			{
				if (guisPause[i]) guisPause[i]->Update();
			}
		}

		if (currentPauseMenu == CurrentPauseMenu::Controls)
		{
			for (int i = 0; i < guisControls.count(); i++)
			{
				if (guisControls[i]) guisControls[i]->Update();
			}
		}

		if (currentPauseMenu == CurrentPauseMenu::Settings)
		{
			for (int i = 0; i < guisSettingsP.count(); i++)
			{
				if (guisSettingsP[i]) guisSettingsP[i]->Update();
			}
		}
	}


	if (currentPauseMenu == CurrentPauseMenu::Pause)
	{
		if (ResumeBUT->doAction)
		{
			app->isPause = false;
			ResumeBUT->doAction = false;
		}

		if (ControlsBUT->doAction)
		{
			currentPauseMenu = CurrentPauseMenu::Controls;
			ControlsBUT->doAction = false;
		}

		if (SettingsBUT->doAction)
		{
			currentPauseMenu = CurrentPauseMenu::Settings;
			SettingsBUT->doAction = false;
		}

		if (GiveUpBUT->doAction)
		{
			app->scene->ChangeCurrentSceneRequest(MAIN_MENU);//et porta al hall
			app->isPause = false;
			GiveUpBUT->doAction = false;
		}

		if (QuitBUT->doAction)
		{
			app->scene->ChangeCurrentSceneRequest(MAIN_MENU);
			app->isPause = false;
			QuitBUT->doAction = false;
		}
	}

	if (currentPauseMenu == CurrentPauseMenu::Settings)
	{
		if (CloseSettingsBUT->doAction)
		{
			currentPauseMenu = CurrentPauseMenu::Pause;
			CloseSettingsBUT->doAction = false;
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

	if (currentPauseMenu == CurrentPauseMenu::Controls)
	{
		if (CloseControlsBUT->doAction)
		{
			currentPauseMenu = CurrentPauseMenu::Pause;
			CloseControlsBUT->doAction = false;
		}
	}

	Scene::Update();

	return true;
}

bool HUDInGame::PostUpdate()
{
	app->renderer->AddRectRenderQueue(hpRect, { 155, 0, 0, 255 }, true, 3, 2.0f, 0.0f);
	app->renderer->AddRectRenderQueue(hpRect, { 155, 155, 155, 255 }, false, 3, 3.0f, 0.0f);


	app->renderer->AddRectRenderQueue(spell1, { 155, 155, 155, 255 }, false, 3, 2.0f, 0.0f);
	app->renderer->AddRectRenderQueue(spell2, { 155, 155, 155, 255 }, false, 3, 2.0f, 0.0f);
	app->renderer->AddRectRenderQueue(spell3, { 155, 155, 155, 255 }, false, 3, 2.0f, 0.0f);
	app->renderer->AddRectRenderQueue(spell4, { 155, 155, 155, 255 }, false, 3, 2.0f, 0.0f);


	if (app->isPause)
	{
		if (currentPauseMenu == CurrentPauseMenu::Pause)
		{
			for (int i = 0; i < guisPause.count(); i++)
			{
				if (guisPause[i]) guisPause[i]->PostUpdate();
			}
		}

		if (currentPauseMenu == CurrentPauseMenu::Controls)
		{
			for (int i = 0; i < guisControls.count(); i++)
			{
				if (guisControls[i]) guisControls[i]->PostUpdate();
			}
		}

		if (currentPauseMenu == CurrentPauseMenu::Settings)
		{
			for (int i = 0; i < guisSettingsP.count(); i++)
			{
				if (guisSettingsP[i]) guisSettingsP[i]->PostUpdate();
			}
		}

		if (currentPauseMenu == CurrentPauseMenu::Pause)
		{
			app->renderer->AddRenderObjectRenderQueue(PauseBG);
		}

		if (currentPauseMenu == CurrentPauseMenu::Controls)
		{
			if(app->input->usingGameController)
				app->renderer->AddRenderObjectRenderQueue(Controls2);
			else
				app->renderer->AddRenderObjectRenderQueue(Controls1);
		}

		if (currentPauseMenu == CurrentPauseMenu::Settings)
		{
			app->renderer->AddRenderObjectRenderQueue(SettingsBG);
		}
	}

	Scene::PostUpdate();

	return true;
}

bool HUDInGame::CleanUp()
{
	Scene::CleanUp();

	return true;
}