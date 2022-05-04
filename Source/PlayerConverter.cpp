#include "PlayerConverter.h"
#include "ModuleTextures.h"
#include "Trigger.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "HubScene.h"
#include "ModuleScene.h"
#include "SceneGame.h"
#include "ModuleMap.h"

PlayerConverter::PlayerConverter(iPoint pos,std::string name):GameObject(name,"PlayerConverter")
{
	renderObjects[0].InitAsTexture(app->textures->Load("Assets/Sprites/Player/Icons/RevenantSpriteInHub.png"),pos, { 0,0,0,0 },1.5f,2,0.8f);
	renderObjects[1].InitAsTexture(app->textures->Load("Assets/Sprites/Player/Icons/SageSpriteInHub.png"), pos, { 0,0,0,0 }, 0.85f, 2, 0.8f);//870,1970

	trigger = new Trigger({ renderObjects[0].destRect.x + 23,renderObjects[0].destRect.y + 24}, 25, 25, this, "ConvertClassTrigger", false);

	b2Filter filter;
	filter.categoryBits = app->physics->TRIGGER_LAYER;
	trigger->pBody->body->GetFixtureList()->SetFilterData(filter);

	this->position = pos;
	
	SceneGame* scene = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	player = scene->player;
}

PlayerConverter::~PlayerConverter()
{
}

void PlayerConverter::Start()
{
	text = new Text({this->position.x - 5,this->position.y - 10}, " ");
	text->ChangeDrawMode();
}


void PlayerConverter::Update()
{
	if (ReadyToChangeClass)
	{
		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			PlayerClass newClass = player->playerClass == PlayerClass::REVENANT ? PlayerClass::SAGE : PlayerClass::REVENANT;
			player->ChangeClass(newClass);
		}
		
	}
}

void PlayerConverter::PostUpdate()
{
	if (exterior && !app->map->roof) return;
	if (!exterior && app->map->roof) return;

	if (player->playerClass == PlayerClass::REVENANT)
	{
		app->renderer->AddRenderObjectRenderQueue(renderObjects[0]);
	}
	else 
	{
		app->renderer->AddRenderObjectRenderQueue(renderObjects[1]);
	}
}

void PlayerConverter::CleanUp()
{
}

void PlayerConverter::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr)return;

	if (col->gameObject->name == "Player" && !ReadyToChangeClass)
	{
		LOG("Ready to change class");
		
		ReadyToChangeClass = true;

		text->SetText("Press F to change player class");
	}
	
}

void PlayerConverter::OnTriggerExit(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->name == "Player")
	{
		LOG(" Not ready to change class");

		ReadyToChangeClass = false;

		text->SetText(" ");
	}
	
}
