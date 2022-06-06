#include "OrbHP.h"
#include "SceneGame.h"
#include "Application.h"
#include "PlayerController.h"
#include "ModuleScene.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"


OrbHP::OrbHP(iPoint position):GameObject("OrbHP","OrbHP")
{
	this->listenTo[0] = GameEvent::PLAYER_DIE;
	app->events->AddListener(this);

	this->position = position;
	trigger = new Trigger({position.x, position.y}, 10, this, "OrbHPTrigger", false);
	b2Filter filter;
	filter.categoryBits = app->physics->TRIGGER_LAYER;
	filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER;
	trigger->pBody->body->GetFixtureList()->SetFilterData(filter);
	/*triggerDetectPlayer = new Trigger({ position.x + 4,position.y + 10 }, 50, this, "SoulsDetectPlayer", true);
	triggerDetectPlayer->pBody->body->GetFixtureList()->SetFilterData(filter);*/

	//HPOrb.InitAsTexture(app->textures->Load("Assets/Sprites/UI/HP.png"), {position.x,position.y}, {0,0,0,0}, 0.25f, 2);

	InitRenderObjectWithXml("orbHP");

	SceneGame* scene = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	player = scene->player;
}

OrbHP::~OrbHP()
{

}

void OrbHP::Start()
{

}

void OrbHP::PreUpdate()
{

}

void OrbHP::Update()
{

}

void OrbHP::PostUpdate()
{
	//app->renderer->AddRectRenderQueue({0,0,50,50}, {255,0,0,255}, true, 2);
	//app->renderer->AddRenderObjectRenderQueue(HPOrb);

	GameObject::PostUpdate();
}

void OrbHP::CleanUp()
{
	app->events->RemoveListener(this);
}

void OrbHP::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	if (trigger == "OrbHPTrigger" && col->gameObject->name == "Player")
	{
		LOG("+ HP");
		PlusHP();
		pendingToDelete = true;
	}
}

void OrbHP::GameEventTriggered(GameEvent id)
{
	this->player = nullptr;
}


void OrbHP::PlusHP()
{
	if(player->hpPlayer > player->hpMax)
		player->hpPlayer += 20;

	if(player->hpPlayer < player->hpMax)
		player->hpPlayer = player->hpMax;

}
