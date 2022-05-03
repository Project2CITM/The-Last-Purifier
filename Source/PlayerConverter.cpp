#include "PlayerConverter.h"
#include "ModuleTextures.h"
#include "Trigger.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "HubScene.h"
#include "ModuleScene.h"
#include "SceneGame.h"

PlayerConverter::PlayerConverter(std::string name):GameObject(name,"PlayerConverter")
{
	renderObjects[0].InitAsTexture(app->textures->Load("Assets/Sprites/Player/Icons/RevenantSpriteInHub.png"), { 870,1970 }, { 0,0,0,0 },1.5f,2,0.8f);
	renderObjects[1].InitAsTexture(app->textures->Load("Assets/Sprites/Player/Icons/SageSpriteInHub.png"), { 870,1970 }, { 0,0,0,0 }, 0.85f, 2, 0.8f);

	trigger = new Trigger({ renderObjects[0].destRect.x + 23,renderObjects[0].destRect.y + 24}, 25, 25, this, "ConvertClassTrigger", false);

	b2Filter filter;
	filter.categoryBits = app->physics->TRIGGER_LAYER;
	trigger->pBody->body->GetFixtureList()->SetFilterData(filter);
	

	pointerscene = (HubScene*)app->scene->scenes[app->scene->currentScene];
}

PlayerConverter::~PlayerConverter()
{
}

void PlayerConverter::Start()
{
	text = new Text({865,1960}, " ");
	text->ChangeDrawMode();
}

void PlayerConverter::PreUpdate()
{
	pointerscene->isChangingPlayer = false;
}

void PlayerConverter::Update()
{
	if (ReadyToChangeClass)
	{
		if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN && pointerscene->isChangingPlayer == false)
		{
			pointerscene->ChangePlayer();
			
		}
		
	}
}

void PlayerConverter::PostUpdate()
{
	if (pointerscene->currentclass == PlayerClass::REVENANT)
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

	if (col->gameObject->name == "Player")
	{
		LOG("Ready to change class");
		
		ReadyToChangeClass = true;
	}
	text->SetText("Press N to change player class");
}

void PlayerConverter::OnTriggerExit(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->name == "Player")
	{
		LOG(" Not ready to change class");

		ReadyToChangeClass = false;
	}
	text->SetText(" ");
}
