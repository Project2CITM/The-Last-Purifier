#include "Kaboom.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "SceneGame.h"
#include "Player.h"
#include "ModuleRender.h"
#include "DamageArea.h"

Kaboom::Kaboom(iPoint pos) :Enemy("kaboom")
{
	// Get player pointer
	SceneGame* sceneGame = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	player = sceneGame->player->controller;

	// Init general value
	this->position = pos;

	health = 20;

	moveSpeed = 4;

	damage = 15;

	// Init texture
	InitRenderObjectWithXml("ghoul");

	// Init StateMachine
	InitStateMachine();

	// Init Animation
	InitAnimation();

	// Init physBody 
	InitPhysics();
}

Kaboom::~Kaboom()
{
}

void Kaboom::PreUpdate()
{
}

void Kaboom::Update()
{
}

void Kaboom::PostUpdate()
{
}

void Kaboom::Hit(int damage)
{
}

void Kaboom::OnTriggerEnter(std::string trigger, PhysBody* col)
{
}

void Kaboom::OnTriggerExit(std::string trigger, PhysBody* col)
{
}

void Kaboom::Die()
{
}

void Kaboom::UpdateStates()
{
}

void Kaboom::InitAnimation()
{
	// Create animations
	for (int i = 0; i < 4; i++)
	{
		// Idle anim initialize
		animations[(int)KaboomState::IDLE].PushBack({ 32 * i, 0, 32, 32 });
		animations[(int)KaboomState::IDLE].loop = true;
	}

	for (int i = 0; i < 8; i++)
	{
		// Run anim initialize
		animations[(int)KaboomState::RUN].PushBack({ 32 * i, 32, 32, 32 });
		animations[(int)KaboomState::RUN].loop = true;
	}

	for (int i = 0; i < 6; i++)
	{
		// Attack anim initialize
		animations[(int)KaboomState::ATTACK].PushBack({ 32 * i, 64, 32, 32 });
		animations[(int)KaboomState::ATTACK].loop = false;
	}

	for (int i = 0; i < 4; i++)
	{
		// Hit anim initialize
		animations[(int)KaboomState::HIT].PushBack({ 32 * i, 96, 32, 32 });
		animations[(int)KaboomState::HIT].loop = false;
	}

	for (int i = 0; i < 6; i++)
	{
		// Die anim initialize
		animations[(int)KaboomState::DIE].PushBack({ 32 * i, 128, 32, 32 });
		animations[(int)KaboomState::DIE].loop = false;
	}

	for (int i = 0; i < (int)KaboomState::MAX; i++)
	{
		animations[i].hasIdle = false;
		animations[i].speed = 0.3f;
	}
}

void Kaboom::InitStateMachine()
{
	stateMachine.AddState("Idle", 0);
	stateMachine.AddState("Run", 0);
	stateMachine.AddState("Attack", 1);
	stateMachine.AddState("Hit", 2, 35);
	stateMachine.AddState("Die", 3);

	stateMachine.ChangeState((int)KaboomState::IDLE);
}

void Kaboom::InitPhysics()
{
	b2Filter filter;

	filter.categoryBits = app->physics->TRIGGER_LAYER;

	// Detect player 
	detectTrigger = new Trigger(position, 16, 12, this, "EnemyDetectPlayer");

	detectTrigger->pBody->body->GetFixtureList()->SetFilterData(filter);

	// Hit Player

	damageTrigger = new Trigger(position, 12, this, "kaboom");

	damageTrigger->tag = "Enemy";

	// Attack Trigger

	filter.categoryBits = app->physics->ENEMY_LAYER; // Who am I

	filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER; // Who will coll with me

	attack = new DamageArea(position, 10, 18, &damage);

	attack->pBody->body->SetActive(false);

	attack->pBody->body->GetFixtureList()->SetFilterData(filter);

	// Body
	filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER & ~app->physics->PLAYER_LAYER;

	pBody = app->physics->CreateCircle(position.x, position.y, 12, this, false, b2_dynamicBody, app->physics->ENEMY_LAYER);

	pBody->body->GetFixtureList()->SetFilterData(filter);
}

void Kaboom::DoAttack()
{
}

void Kaboom::DoRun()
{
}
