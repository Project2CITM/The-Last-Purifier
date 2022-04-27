#include "Souls.h"
#include "Trigger.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SpellSpawnManager.h"
#include "SceneGame.h"
#include "ModuleScene.h"
#include "PlayerController.h"

Souls::Souls(iPoint position, int soulsAmount):GameObject("Souls", "Souls")
{
	this->position = position;
	this->soulsAmount = soulsAmount;
	trigger = new Trigger({ position.x+4,position.y+10 }, 10, this, "SoulsTrigger", true);
	triggerDetectPlayer = new Trigger({ position.x + 4,position.y + 10 }, 50, this, "SoulsDetectPlayer", true);
	InitRenderObjectWithXml("soul");

	for (int i = 0; i < 8; i++)
	{
		idleAnim.PushBack({ 50 * i, 0, 50, 105 });
	}
	idleAnim.loop = true;
	idleAnim.speed = 0.1;
	idleAnim.hasIdle = false;

	SceneGame* scene = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	player = scene->player;
}

Souls::~Souls()
{
}

void Souls::Start()
{
}

void Souls::PreUpdate()
{
}

void Souls::Update()
{
	if (!player || !detectPlayer) return;

	// Update Y
	//int targetPosY = player->controller->GetPosition().y;

	//int targetPosX = player->controller->GetPosition().x;

	//iPoint distance = { targetPosX,targetPosY };

	//distance.Normalize();

	//iPoint minSpeed = distance * (8 / followDelay);

	//position += distance * (8 / followDelay);
}

void Souls::PostUpdate()
{
	idleAnim.Update();

	renderObjects[0].section = idleAnim.GetCurrentFrame();

	if (player->hpPlayer <= 0) player = nullptr;

	GameObject::PostUpdate();
}

void Souls::CleanUp()
{
}



void Souls::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (trigger == "SoulsDetectPlayer") detectPlayer = true;

	if (trigger == "SoulsTrigger" && col->gameObject->name == "Player")
	{
		LOG("Enter");
		//Colocar velocidad hacia jugador
		if (player != nullptr) player->AddSouls(soulsAmount);
		pendingToDelete = true;
		//isDie = true;
	}

}

