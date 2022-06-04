#include "HubScene.h"
#include "PlayerController.h"
#include "PlayerRevenant.h"
#include "PlayerSage.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleMap.h"
#include "ModulePhysics.h"
#include "SceneSwitch.h"
#include "ModuleScene.h"
#include "IntLabel.h"
#include "PlayerCombat.h"
#include "ModuleEvents.h"
#include "ModuleAudio.h"
#include "Minimap.h"

HubScene::HubScene() : SceneGame("HubScene")
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
			case 0: //Walls ^^

				g = new GameObject("wall", "Wall");
				g->pBody = app->physics->CreateRectangle(pos, width, height, g);
				g->pBody->body->SetType(b2BodyType::b2_staticBody);
				g->pBody->body->GetFixtureList()->SetFriction(0);

				break;
			case 1:

				g = new SceneSwitch(LEVEL_1, "sceneSwitch", "SceneSwitch");
				g->pBody = app->physics->CreateRectangleSensor(pos, width, height, g);

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
		g->adjustToGrid = true;
	}

	return true;
}

bool HubScene::Start()
{
	app->map->Load(HUB_MAP);
	app->map->roof = false;
	//Map

	/*Player related*/
	PlayerClass playerClass;
	pugi::xml_document playerStats;
	pugi::xml_parse_result result = playerStats.load_file("PlayerStats.xml");
	if (result == NULL)
	{
		LOG("Could not load xml file: %s. pugi error: %s", "PlayerStats.xml", result.description());
	}
	else
	{
		playerClass = (PlayerClass)playerStats.child("stats").child("currentClass").attribute("class").as_int();
	}

	if (playerClass == PlayerClass::REVENANT) player = new PlayerRevenant();
	else player = new PlayerSage();

	revenantTree = ClassTree::GetInstance();

	//Starts
	hudInGame = new HUDInGame();
	hudInGame->Start();

	PlayerChangeClass = new PlayerConverter({ 870,1970 }, "ClassChanger");

	PlayerChangeClassBeforeRun = new PlayerConverter({ 1478,281 }, "ClassChangerBeforeRun");

	Scene::Start();

	player->controller->SetPosition(startPosition);

	app->renderer->camera->SetPosition(startPosition);

	app->renderer->camera->SetTarget(player->controller);
	
	InitScene();
	//NPC creators
	npc1 = new NPC("purifier10", {1228,1717});//Interior castillo o perdido por ah�
	npc1->Start();

	npc2 = new NPC("purifier9", {478,1470});//Cementerio
	npc2->Start();

	npc3 = new NPC("purifier8", {1228,493});//Puerta Start Run
	npc3->Start();

	npc4 = new NPC("purifier6", {928,1867});//Puerta Castillo
	npc4->Start();

	quest2 = new Quest("quest2", { 1181,1314 });
	quest2->Start();
	quest3 = new Quest("quest3", { 1205,2014 });
	quest3->Start();
	quest4 = new Quest("quest4", { 191,1823 });
	quest4->Start();
/*	quest5 = new Quest("quest5", {940,1900});
	quest5->Start();*/
	std::string sentenceInput;
	if (app->input->usingGameController) sentenceInput = "<Press A>";
	else sentenceInput = "<Press F>";

	revenantInstructor = new Instructor(sentenceInput, "RevenantInstructor", { 1016, 855 }, PlayerClass::REVENANT);
	revenantInstructor->setIndoors();

	sageInstructor = new Instructor(sentenceInput, "SageInstructor", { 550, 1395 }, PlayerClass::SAGE);

	//Labels
	IntLabel* int_lbl_Revenant = new IntLabel("REVENANT INSTRUCTOR", "Lbl_Revenant", { 1014, 939 }, { 255 , 20 },  150);
	IntLabel* int_lbl_Sage = new IntLabel("SAGE INSTRUCTOR", "Lbl_Sage", { 553, 1510 }, { 270 , 20 }, 180);
	IntLabel* int_lbl_Gate = new IntLabel("DOOM'S GATE", "Lbl_Doom", { 1271, 211 }, { 295 , 20 }, 200);

	//Music
	app->audio->PlayMusic("Audio/Music/HubMusic.ogg", 2.0f, false);

	//PlayerClassChanger
	PlayerChangeClass->setOutside();

	PlayerChangeClassBeforeRun->setInside();

	//MiniMap
	miniMap = new MiniMap();
	miniMap->Init(true);

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
	
	RELEASE(miniMap);

	revenantTree->ReleaseInstance();
	
	Scene::CleanUp();
	return false;
}

bool HubScene::PreUpdate()
{
	//Inputs
	if (!app->isMenu)
	{
		//Pause Menu
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->GetControllerButton(BUTTON_START) == KEY_DOWN) app->TogglePause(!app->isPause);
		//Roof switch
		if (app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN) app->map->roof = !app->map->roof;

		if (app->input->usingGameController)
		{
			revenantInstructor->sentence = "<Press A>";
			sageInstructor->sentence = "<Press A>";
		}
		else
		{
			revenantInstructor->sentence = "<Press F>";
			sageInstructor->sentence = "<Press F>";
		}

		//MiniMap resize
		miniMap->SetScale((app->input->GetKey(SDL_SCANCODE_TAB) == KEY_REPEAT || app->input->GetControllerButton(BUTTON_BACK) == KEY_REPEAT) ? 2 : 1);
	}

	/*if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		iPoint aux = player->controller->GetPosition();
		std::cout << "{ " << aux.x << ", " << aux.y << " }" << std::endl;
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		player->controller->SetPosition({ 926, 1579 });
	}*/
	//PreUpdates
	hudInGame->PreUpdate();

	revenantTree->PreUpdate();

	Scene::PreUpdate();

	return true;
}

bool HubScene::Update()
{
	//Updates
	hudInGame->Update();
	
	int x = player->controller->GetPosition().x;
	int y = player->controller->GetPosition().y;
	

	revenantTree->Update();	

	//Quest
	if (npc4->speak == true && !done1) {
		quest2->active = false;
		quest2->Start();
		done1 = true;
	}
	if (npc2->speak == true && !done2) {
		quest3->active = false;
		quest3->Start();
		done2 = true;
	}
	if (npc3->speak == true && !done3) {
		quest4->active = false;
		quest4->Start();
		done3 = true;
	}
	Scene::Update();
	return true;
}

bool HubScene::PostUpdate()
{
	//PostUpdates
	hudInGame->PostUpdate();

	revenantTree->PostUpdate();

	miniMap->MiniMapPrint(iPoint(485, -95), player->controller->GetPosition());

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

