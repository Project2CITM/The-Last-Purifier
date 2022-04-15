#include "HubScene.h"
#include "PlayerController.h"
#include "PlayerRevenant.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleMap.h"
#include "ModulePhysics.h"

HubScene::HubScene() : Scene("HubScene")
{

}

HubScene::~HubScene()
{

}

bool HubScene::InitScene()
{
	int width = 16;
	int height = 16;

	for (int i = 0; i < app->map->mapObjects.count(); i++)
	{
		iPoint pos = { app->map->mapObjects[i].position.x + 16, app->map->mapObjects[i].position.y + 16 };
		GameObject* g = nullptr;


		switch (app->map->mapObjects[i].id)
		{
			case 0: //Walls
				
				g = new GameObject("wall", "Wall");
				g->pBody = app->physics->CreateRectangle(pos, width, height, g);
				g->pBody->body->SetType(b2BodyType::b2_staticBody);
				g->pBody->body->GetFixtureList()->SetFriction(0);
				g->adjustToGrid = true;

				break;
			case 2: //Door IN

				g = new GameObject("hubDoorIN", "HubDoorIN");
				g->pBody = app->physics->CreateRectangleSensor(pos, width, height, g);

				break;
			case 3: //Door Out

				g = new GameObject("hubDoorOUT", "HubDoorOUT");
				g->pBody = app->physics->CreateRectangleSensor(pos, width, height, g);

				break;
		}
	}

	return true;
}

bool HubScene::Start()
{
	app->map->Load(HUB_MAP);
		
	//Map

	/*Player related*/
	player = new PlayerRevenant();
	app->renderer->camera->SetTarget(player->controller);
	

	//Starts
	hudInGame = new HUDInGame();
	hudInGame->Start();

	Scene::Start();
	player->controller->SetPosition(startPosition);

	InitScene();

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
	return false;
}

bool HubScene::PreUpdate()
{
	//Inputs
		//Menu button
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) app->TogglePause(!app->isPause);
	if (app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN) app->map->roof = !app->map->roof;

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		iPoint aux = player->controller->GetPosition();
		std::cout << "X-> " << aux.x << " | Y-> " << aux.y << std::endl;
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		player->controller->SetPosition({ 926, 1579 });
	}
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