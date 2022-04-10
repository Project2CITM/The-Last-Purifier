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

	hpRect = { app->renderer->camera->x + 15, app->renderer->camera->y + 10, 200, 10 };

	miniMap = { app->renderer->camera->x + 535, app->renderer->camera->y + 5, 100, 100 };

	spell1 = { app->renderer->camera->x + 225, app->renderer->camera->y + 314, 30, 40 };
	spell2 = { app->renderer->camera->x + 275, app->renderer->camera->y + 314, 30, 40 };
	spell3 = { app->renderer->camera->x + 325, app->renderer->camera->y + 314, 30, 40 };
	spell4 = { app->renderer->camera->x + 375, app->renderer->camera->y + 314, 30, 40 };

	pause = { app->renderer->camera->x, app->renderer->camera->y, 500, 300 };

	resumeBUT = { app->renderer->camera->x, app->renderer->camera->y };
	settingsBUT = { app->renderer->camera->x, app->renderer->camera->y };
	controlsBUT = { app->renderer->camera->x, app->renderer->camera->y };
	quitBUT = { app->renderer->camera->x, app->renderer->camera->y };
	giveUpBUT = { app->renderer->camera->x, app->renderer->camera->y };

	ResumeBUT = new GUIButton(resumeBUT, 75, 25, MenuButton::INGAMEPUASE, "Assets/Sprites/UI/PlayBUT.png");


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
	if (app->isPause)
	{
		for (int i = 0; i < guisPause.count(); i++)
		{
			if (guisPause[i]) guisPause[i]->Update();
		}
	}

	Scene::Update();

	return true;
}

bool HUDInGame::PostUpdate()
{
	app->renderer->AddRectRenderQueue(hpRect, { 155, 0, 0, 255 }, true, 3, 2.0f, 0.0f);
	app->renderer->AddRectRenderQueue(hpRect, { 155, 155, 155, 255 }, false, 4, 2.0f, 0.0f);


	app->renderer->AddRectRenderQueue(spell1, { 155, 155, 155, 255 }, false, 4, 2.0f, 0.0f);
	app->renderer->AddRectRenderQueue(spell2, { 155, 155, 155, 255 }, false, 4, 2.0f, 0.0f);
	app->renderer->AddRectRenderQueue(spell3, { 155, 155, 155, 255 }, false, 4, 2.0f, 0.0f);
	app->renderer->AddRectRenderQueue(spell4, { 155, 155, 155, 255 }, false, 4, 2.0f, 0.0f);


	if (app->isPause)
	{
		for (int i = 0; i < guisPause.count(); i++)
		{
			if (guisPause[i]) guisPause[i]->PostUpdate();
		}
		//app->renderer->AddRectRenderQueue(pause, { 140, 215, 0, 255 }, true, 5, 2.0f, 0.0f);

		if (ResumeBUT->doAction)
		{
			app->isPause = false;
			ResumeBUT->doAction = false;
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