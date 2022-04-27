#include "Souls.h"
#include "Trigger.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SpellSpawnManager.h"
#include "SceneGame.h"
#include "ModuleScene.h"

Souls::Souls(iPoint position, int soulsAmount):GameObject("Souls", "Souls")
{
	this->position = position;
	this->soulsAmount = soulsAmount;
	trigger = new Trigger({ position.x+4,position.y+10 }, 10, this, "SoulsTrigger", false);
	InitRenderObjectWithXml("soul");


	for (int i = 0; i < 8; i++)
	{
		idleAnim.PushBack({ 50 * i,0,50,105 });
	}
	idleAnim.loop = true;
	idleAnim.speed = 0.1;
	idleAnim.hasIdle = false;

}

Souls::~Souls()
{
}

void Souls::Start()
{
	SceneGame* scene = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	player = scene->player;

}

void Souls::PreUpdate()
{
}

void Souls::Update()
{
}

void Souls::PostUpdate()
{

	idleAnim.Update();
	renderObjects[0].section = idleAnim.GetCurrentFrame();
	GameObject::PostUpdate();
	
}

void Souls::CleanUp()
{
}

void Souls::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->name == "Player")
	{
		LOG("Enter");
		//Colocar velocidad hacia jugador
		if (player != nullptr) player->AddSouls(soulsAmount);
		pendingToDelete = true;
		//isDie = true;
	}

}

