#include "Worm.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "SceneGame.h"
#include "Player.h"
#include "ModuleRender.h"

Worm::Worm(iPoint pos) : Enemy("worm")
{
	// Get player pointer
	SceneGame* sceneGame = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	player = sceneGame->player->controller;

	// Init general value
	this->position = pos;

	health = 20;

	moveSpeed = 2;

	damage = 5;

	groundCoolDown = 20; // frame

	unergroundCoolDown = 60; // frame

	// Init texture
	InitRenderObjectWithXml("worm");

	// Init StateMachine
	InitStateMachine();

	// Init Animation
	InitAnimation();

	// Init physBody 
	InitPhysics();
}

Worm::~Worm()
{
}

void Worm::PreUpdate()
{
}

void Worm::Update()
{
}

void Worm::PostUpdate()
{
	SDL_RendererFlip flip = GetLinearVelocity().x > 0 ? SDL_FLIP_NONE : GetLinearVelocity().x < 0 ? SDL_FLIP_HORIZONTAL : renderObjects[0].flip;

	animations[stateMachine.GetCurrentState()].Update();

	for (int i = 0; i < 3; i++) renderObjects[i].flip = flip;

	renderObjects[0].section = animations[stateMachine.GetCurrentState()].GetCurrentFrame();

	Enemy::PostUpdate();
}

void Worm::Hit(int damage)
{
	stateMachine.ChangeState((int)WormState::HIT);

	animations[stateMachine.GetCurrentState()].Reset();

	renderObjects[0].SetColor({ 255,164,164,100 });
}

void Worm::OnTriggerEnter(std::string trigger, PhysBody* col)
{
}

void Worm::OnTriggerExit(std::string trigger, PhysBody* col)
{
}

void Worm::GoDie()
{
}

void Worm::UpdateStates()
{
	SetLinearVelocity(b2Vec2{ 0,0 });

	switch (stateMachine.GetCurrentState())
	{
	case (int)WormState::IDLE:
	{
		if (groundCoolDown <= 0)
		{
			ResetGroundsCoolDown();

			return;
		}

		groundCoolDown--;
	}
	break;
	case (int)WormState::UNDERGROUND:
	{
		if (unergroundCoolDown <= 0)
		{
			ResetGroundsCoolDown();

			return;
		}

		unergroundCoolDown--;
	}
		break;
	case (int)WormState::INGROUND:
	{
		if (animations[stateMachine.GetCurrentState()].HasFinished()) stateMachine.ChangeState((int)WormState::UNDERGROUND);
	}
	break;
	case (int)WormState::OUTGROUND:
	{
		if (animations[stateMachine.GetCurrentState()].HasFinished()) stateMachine.ChangeState((int)WormState::IDLE);
	}
	break;
	case (int)WormState::HIT:
	{
		if (!animations[stateMachine.GetCurrentState()].HasFinished()) return;

		renderObjects[0].SetColor({ 255,255,255,255 });

		stateMachine.ChangeState((int)WormState::IDLE);
	}
	break;
	case (int)WormState::DIE:
	{
		if (animations[stateMachine.GetCurrentState()].HasFinished()) Enemy::Die();
	}
	break;
	}
}

void Worm::InitAnimation()
{
	// Create animations
	
	// Idle anim initialize
	for (int i = 0; i < 8; i++)	animations[(int)WormState::IDLE].PushBack({ 32 * i, 0, 32, 32 });
	animations[(int)WormState::IDLE].loop = true;

	// InGround anim initialize
	for (int i = 0; i < 6; i++)	animations[(int)WormState::INGROUND].PushBack({ 32 * i, 64, 32, 32 });
	animations[(int)WormState::INGROUND].loop = false;

	// Burried in ground anim initialize
	animations[(int)WormState::UNDERGROUND].PushBack({ 0, 0, 1, 1 });
	animations[(int)WormState::UNDERGROUND].loop = true;

	// OutGround anim initialize
	for (int i = 0; i < 5; i++)	animations[(int)WormState::OUTGROUND].PushBack({ 32 * i, 96, 32, 32 });
	animations[(int)WormState::OUTGROUND].loop = false;

	// Hit anim initialize
	for (int i = 0; i < 4; i++) animations[(int)WormState::HIT].PushBack({ 32 * i, 128, 32, 32 });
	animations[(int)WormState::HIT].loop = false;

	// GoDie anim initialize
	for (int i = 0; i < 6; i++) animations[(int)WormState::DIE].PushBack({ 32 * i, 160, 32, 32 });
	animations[(int)WormState::DIE].loop = false;

	// Init general value
	for (int i = 0; i < (int)WormState::MAX; i++)
	{
		animations[i].hasIdle = false;
		animations[i].speed = 0.3f;
	}
}

void Worm::InitStateMachine()
{
	stateMachine.AddState("Idle", 0);
	stateMachine.AddState("InGround", 1);
	stateMachine.AddState("UnderGround", 1);
	stateMachine.AddState("OutGround", 1);
	stateMachine.AddState("Hit", 0);
	stateMachine.AddState("Die", 2);

	stateMachine.ChangeState((int)WormState::IDLE);
}

void Worm::InitPhysics()
{
	// Hit Player or get hit
	b2Filter filter;

	filter.categoryBits = app->physics->ENEMY_LAYER; // Who am I

	filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER; // Who will coll with me

	damageTrigger = new Trigger(position, 12, this, "worm");

	damageTrigger->tag = "Enemy";

	damageTrigger->pBody->body->GetFixtureList()->SetFilterData(filter);

	// Body
	b2Filter filterB;

	filterB.categoryBits = app->physics->ENEMY_LAYER;

	filterB.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER & ~app->physics->PLAYER_LAYER;

	pBody = app->physics->CreateCircle(position.x, position.y, 12, this, false, b2_dynamicBody, app->physics->ENEMY_LAYER);

	pBody->body->GetFixtureList()->SetFilterData(filterB);
}

void Worm::DoOutGround()
{
}


void Worm::ResetGroundsCoolDown()
{
	groundCoolDown = 20;
	unergroundCoolDown = 60;
}
