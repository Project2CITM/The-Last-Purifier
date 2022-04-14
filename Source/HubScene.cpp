#include "HubScene.h"
#include "PlayerController.h"
#include "PlayerRevenant.h"
#include "ModuleRender.h"
#include "ModuleInput.h"

HubScene::HubScene() : Scene("HubScene")
{

}

HubScene::~HubScene()
{

}

bool HubScene::Start()
{

	//Map
	/*if (app->map->Load(levelList[0]->file.GetString()) == true)
	{
		int w, h;
		uchar* data = NULL;

		if (app->map->CreateWalkabilityMap(w, h, &data)) app->SetMap(w, h, data);
		app->map->generated = true;
		RELEASE_ARRAY(data);
	}*/

	/*Player related*/
	player = new PlayerRevenant();
	app->renderer->camera->SetTarget(player->controller);

	//Starts
	hudInGame = new HUDInGame();
	hudInGame->Start();

	Scene::Start();
	return true;
}

bool HubScene::CleanUp()
{
	if (player != nullptr)
	{
		player->CleanUp();
		RELEASE(player);
	}


	if (hudInGame != nullptr)
	{
		hudInGame->CleanUp();
		RELEASE(hudInGame);
	}

	Scene::CleanUp();
	return true;
}

bool HubScene::PreUpdate()
{
	//Inputs
		//Menu button
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || app->input->GetControllerButton(BUTTON_START) == KEY_DOWN) app->TogglePause(!app->isPause);

	//PreUpdates
	hudInGame->PreUpdate();

	Scene::PreUpdate();
	return true;
}

bool HubScene::Update()
{
	//Updates
	hudInGame->Update();

	Scene::Update();
	return true;
}

bool HubScene::PostUpdate()
{

	//PostUpdates
	hudInGame->PostUpdate();

	Scene::PostUpdate();
	return true;
}


/*GUI*/
void HubScene::AddGUIPause(GUI* gui)
{
	hudInGame->AddGUIPause(gui);
}

void HubScene::AddGUIControls(GUI* gui)
{
	hudInGame->AddGUIControls(gui);
}

void HubScene::AddGUISettingsP(GUI* gui)
{
	hudInGame->AddGUISettingsP(gui);
}