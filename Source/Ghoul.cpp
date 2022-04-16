#include "Ghoul.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "SceneGame.h"
#include "Player.h"
#include "ModuleRender.h"

Ghoul::Ghoul(iPoint pos) : Enemy("ghoul")
{
	// Get player pointer
	SceneGame* sceneGame = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	player = sceneGame->player->controller;

	// Init texture
	InitRenderObjectWithXml("ghoul");

	// Init StateMachine
	InitStateMachine();

	// Init Animation
	InitAnimation();

	// Init physBody 
	pBody = app->physics->CreateCircle(pos.x, pos.y, 12, this, false, b2_dynamicBody, app->physics->ENEMY_LAYER);

	detectTrigger = new Trigger(position, 16, 12, this, "EnemyDetectPlayer");

	attackTrigger = new Trigger(position, 6, 8, this, "EnemyAttack");

	// Init his position
	this->position = pos;

	// Init life
	health = 20;

	// Init movementSpeed
	moveSpeed = 2;
}

Ghoul::~Ghoul()
{
}

void Ghoul::PreUpdate()
{
	UpdateStates();

	Enemy::PreUpdate();
}

void Ghoul::Update()
{
	stateMachine.Update();

	Enemy::Update();
}

void Ghoul::PostUpdate()
{
	renderObjects[0].flip = GetLinearVelocity().x > 0 ? SDL_FLIP_NONE : GetLinearVelocity().x < 0 ? SDL_FLIP_HORIZONTAL : renderObjects[0].flip;

	animations[stateMachine.GetCurrentState()].Update();

	int frame = animations[stateMachine.GetCurrentState()].getCurrentFrameI();

	renderObjects[0].section = animations[stateMachine.GetCurrentState()].GetCurrentFrame();

	Enemy::PostUpdate();
}

void Ghoul::Hit(int damage)
{
	stateMachine.ChangeState((int)GhoulState::HIT);

	animations[stateMachine.GetCurrentState()].Reset();

	renderObjects[0].SetColor({ 255,164,164,100 });

	Enemy::Hit(damage);
}

void Ghoul::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	if(trigger=="EnemyDetectPlayer")
	{
		if (col->gameObject->name == "Player")
		{
			detectPlayer = true;
		}
	}
}

void Ghoul::OnTriggerExit(std::string trigger, PhysBody* col)
{
	if (trigger == "EnemyDetectPlayer")
	{
		if (col->gameObject->name == "Player")
		{
			printf("player exit\n");
			detectPlayer = false;
		}
	}
}

void Ghoul::Die()
{
	stateMachine.ChangeState((int)GhoulState::DIE);

	detectTrigger->ReleaseParent();

	detectTrigger->pendingToDelete = true;
}

void Ghoul::UpdateStates()
{
	SetLinearVelocity(b2Vec2{ 0,0 });
	
	// printf("%d\n", stateMachine.GetCurrentState());

	switch (stateMachine.GetCurrentState())
	{
	case (int)GhoulState::IDLE:
	{
		if (detectPlayer)
		{
			stateMachine.ChangeState((int)GhoulState::ATTACK);
			animations[stateMachine.GetCurrentState()].Reset();
		}
		else
		{
			fPoint dir = { (float)(player->GetPosition().x - position.x), (float)(player->GetPosition().y - position.y) };
			dir = dir.Normalize();
			SetLinearVelocity(b2Vec2{ (float)(dir.x * moveSpeed),(float)(dir.y * moveSpeed) });
			stateMachine.ChangeState((int)GhoulState::RUN);
		}
	}
	break;
	case (int)GhoulState::RUN:
	{
		fPoint dir = { (float)(player->GetPosition().x - position.x), (float)(player->GetPosition().y - position.y) };
		dir = dir.Normalize();
		SetLinearVelocity(b2Vec2{ (float)(dir.x * moveSpeed),(float)(dir.y * moveSpeed) });
		app->renderer->AddLineRenderQueue(position, player->GetPosition(), false, { 255,255,255,255 }, 2);
		if(detectPlayer)
		{
			stateMachine.ChangeState((int)GhoulState::ATTACK);
			animations[stateMachine.GetCurrentState()].Reset();
		}
	}
	break;
	case (int)GhoulState::ATTACK:
	{
		SetLinearVelocity(b2Vec2{ 0,0 });
		if (animations[stateMachine.GetCurrentState()].HasFinished())
		{
			if (!detectPlayer)
			{
				stateMachine.ChangeState((int)GhoulState::IDLE);
			}
			else
			{
				stateMachine.ChangeState((int)GhoulState::ATTACK);
				animations[stateMachine.GetCurrentState()].Reset();
			}
		}
	}
		break;
	case (int)GhoulState::HIT:
	{
		if (animations[stateMachine.GetCurrentState()].HasFinished())
		{
			stateMachine.ChangeState((int)GhoulState::IDLE);
			renderObjects[0].SetColor({ 255,255,255,255 });
		}
	}	
		break;
	case (int)GhoulState::DIE:
	{
		if (animations[stateMachine.GetCurrentState()].HasFinished())
		{
			Enemy::Die();
		}
	}	
		break;
	}
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
		animations[(int)GhoulState::ATTACK].loop = false;
	}

	for (int i = 0; i < 4; i++)
	{
		// Hit anim initialize
		animations[(int)GhoulState::HIT].PushBack({ 32 * i, 96, 32, 32 });
		animations[(int)GhoulState::HIT].loop = false;
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
}

void Ghoul::InitStateMachine()
{
	stateMachine.AddState("Idle", 0);
	stateMachine.AddState("Run", 0);
	stateMachine.AddState("Attack", 1, 35);
	stateMachine.AddState("Hit", 2, 35);
	stateMachine.AddState("Die", 35);

	stateMachine.ChangeState((int)GhoulState::IDLE);
}