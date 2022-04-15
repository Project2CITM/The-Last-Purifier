#include "Ghoul.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "SceneGame.h"
#include "Player.h"


Ghoul::Ghoul(iPoint pos) : Enemy("ghoul")
{
	// Get player pointer
	SceneGame* sceneGame = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	player = sceneGame->player->controller;

	// Init texture
	InitRenderObjectWithXml("ghoul");

	// Init Animation
	InitAnimation();

	// Init StateMachine
	InitStateMachine();

	// Init physBody 
	pBody = app->physics->CreateCircle(pos.x, pos.y, 12, this, true);

	// Init his position
	this->position = pos;

	// Init life
	health = 20;

	// Init movementSpeed
	moveSpeed = 20;
}

Ghoul::~Ghoul()
{
}

void Ghoul::PreUpdate()
{
	switch (stateMachine.GetCurrentState())
	{
	case (int)GhoulState::IDLE:
	{
		iPoint playerPos = player->GetPosition();
		iPoint ghoulPos = GetPosition();
		b2Vec2 vel = GetLinearVelocity();

		int distance = ghoulPos.DistanceTo(playerPos);
		printf("X : %d \t Y : %d\n", ghoulPos.x, ghoulPos.y);

		if (distance < 100)
		{
			//iPoint dir = player->GetPosition() - position;
			//dir.Normalize();
			//SetLinearVelocity(dir * 0.1f);
		}
	}
		break;
	case (int)GhoulState::RUN:
		break;
	case (int)GhoulState::ATTACK:
		break;
	case (int)GhoulState::HIT:
		break;
	case (int)GhoulState::DIE:
		break;
	}

	Enemy::PreUpdate();
}

void Ghoul::Update()
{
	Enemy::Update();
}

void Ghoul::PostUpdate()
{
	animations[(int)currentState].Update();

	int frame = animations[(int)currentState].getCurrentFrameI();

	renderObjects[0].section = animations[(int)currentState].GetCurrentFrame();

	Enemy::PostUpdate();
}

void Ghoul::Hit(int damage)
{
	stateMachine.ChangeState((int)GhoulState::HIT);

	Enemy::Hit(damage);
}

void Ghoul::Die()
{
	stateMachine.ChangeState((int)GhoulState::DIE);

	Enemy::Die();
}

void Ghoul::InitAnimation()
{
	// Create animations
	for (int i = 0; i < 4; i++)
	{
		// Idle anim initialize
		animations[(int)GhoulState::IDLE].PushBack({ 32 * i, 0, 32, 32 });
		animations[(int)GhoulState::IDLE].loop = true;
	}

	for (int i = 0; i < 8; i++)
	{
		// Run anim initialize
		animations[(int)GhoulState::RUN].PushBack({ 32 * i, 32, 32, 32 });
		animations[(int)GhoulState::RUN].loop = true;
	}

	for (int i = 0; i < 6; i++)
	{
		// Attack anim initialize
		animations[(int)GhoulState::ATTACK].PushBack({ 32 * i, 64, 32, 32 });
		animations[(int)GhoulState::ATTACK].loop = true;
	}

	for (int i = 0; i < 4; i++)
	{
		// Hit anim initialize
		animations[(int)GhoulState::HIT].PushBack({ 32 * i, 96, 32, 32 });
		animations[(int)GhoulState::HIT].loop = true;
	}

	for (int i = 0; i < 6; i++)
	{
		// Die anim initialize
		animations[(int)GhoulState::DIE].PushBack({ 32 * i, 128, 32, 32 });
		animations[(int)GhoulState::DIE].loop = false;
	}

	for (int i = 0; i < (int)GhoulState::MAX; i++)
	{
		animations[i].hasIdle = false;
		animations[i].speed = 0.2f;
	}

	currentState = GhoulState::IDLE;
}

void Ghoul::InitStateMachine()
{
	stateMachine.AddState("Idle", 0);
	stateMachine.AddState("Run", 0);
	stateMachine.AddState("Attack", 1, 5);
	stateMachine.AddState("Hit", 2, 5);
	stateMachine.AddState("Die", 3);
}