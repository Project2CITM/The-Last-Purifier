#include "Worm.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "SceneGame.h"
#include "Player.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"

Worm::Worm(iPoint pos) : Enemy("worm")
{
	// Get player pointer
	SceneGame* sceneGame = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	playerController = sceneGame->player->controller;

	// Init general value
	this->position = pos;

	health = 40;

	damage = 10;

	soulsAmount = 14;

	groundCoolDown = 1920; // frame

	unergroundCoolDown = 1920; // frame

	// Init texture
	InitRenderObjectWithXml("worm");

	// Init StateMachine
	InitStateMachine();

	// Init Animation
	InitAnimation();

	// Init physBody 
	InitPhysics();

	hitFX[0] = app->audio->LoadFx("Assets/Audio/SFX/Enemies/Worm/sfx_wormHit1.wav", false);
	hitFX[1] = app->audio->LoadFx("Assets/Audio/SFX/Enemies/Worm/sfx_wormHit2.wav", false);
	digFX = app->audio->LoadFx("Assets/Audio/SFX/Enemies/Worm/earthwormdig.wav", false);
}

Worm::~Worm()
{
	if (damageTrigger != nullptr)
	{
		damageTrigger->Destroy();
		damageTrigger = nullptr;
	}
}

void Worm::PreUpdate()
{
	if (isDie) return;

	wormTimer.Update();
	UpdateStates();
	wormTimer.Reset();
	Enemy::PreUpdate();
}

void Worm::Update()
{
	if (isDie) return;

	stateMachine.Update();

	Enemy::Update();
}

void Worm::PostUpdate()
{
	if (isDie) return;

	SDL_RendererFlip flip = GetLinearVelocity().x > 0 ? SDL_FLIP_NONE : GetLinearVelocity().x < 0 ? SDL_FLIP_HORIZONTAL : renderObjects[0].flip;

	if (!animPause)	animations[stateMachine.GetCurrentState()].Update();

	for (int i = 0; i < 3; i++) renderObjects[i].flip = flip;

	renderObjects[0].section = animations[stateMachine.GetCurrentState()].GetCurrentFrame();

	Enemy::PostUpdate();
}

void Worm::Hit(int damage)
{
	stateMachine.ChangeState((int)WormState::HIT);

	animations[stateMachine.GetCurrentState()].Reset();

	renderObjects[0].SetColor({ 255,164,164,100 });

	Enemy::Hit(damage);
	app->audio->PlayFx(hitFX[rand() % 2]);
}

void Worm::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	Enemy::OnTriggerEnter(trigger, col);
}

void Worm::OnTriggerExit(std::string trigger, PhysBody* col)
{
	Enemy::OnTriggerExit(trigger, col);
}

void Worm::Die(bool spawnPower, bool spawnSouls, bool spawnOrb)
{
	stateMachine.ChangeState((int)WormState::DIE);
}

void Worm::DisableCollisions()
{
	if (damageTrigger != nullptr)
	{
		damageTrigger->Destroy();
		damageTrigger = nullptr;
	}

	if (pBody->body->IsActive()) pBody->body->SetActive(false);
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
			DoInGround();
			
			return;
		}

		groundCoolDown-= wormTimer.getDeltaTime() * 1000;
	}
	break;
		break;
	case (int)WormState::INGROUND:
	{
		if (!animations[stateMachine.GetCurrentState()].HasFinished()) return;
		
		stateMachine.ChangeState((int)WormState::UNDERGROUND);
	}
	break;
	case (int)WormState::UNDERGROUND:
	{
		if (unergroundCoolDown <= 0)
		{
			DoOutGround();

			return;
		}

		unergroundCoolDown-= wormTimer.getDeltaTime() * 1000;
	}
	case (int)WormState::OUTGROUND:
	{
		if (animations[stateMachine.GetCurrentState()].getCurrentFrameI() == 1)
		{
			if (shakeOutGround > 0)
			{
				animPause = true;
				shakeOutGround -= wormTimer.getDeltaTime() * 1000;
			}
			else
			{
				SetTriggeeActive(true);
				animPause = false;
			}
		}

		if (!animations[stateMachine.GetCurrentState()].HasFinished()) return;
		
		stateMachine.ChangeState((int)WormState::IDLE);
		
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
		DisableCollisions();

		if (!animations[stateMachine.GetCurrentState()].HasFinished()) return;
		
		Enemy::Die(true);
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
	for (int i = 0; i < 6; i++)	animations[(int)WormState::INGROUND].PushBack({ 32 * i, 32, 32, 32 });
	animations[(int)WormState::INGROUND].loop = false;

	// Burried in ground anim initialize
	animations[(int)WormState::UNDERGROUND].PushBack({ 0, 0, 1, 1 });
	animations[(int)WormState::UNDERGROUND].loop = true;

	// OutGround anim initialize
	for (int i = 0; i < 5; i++)	animations[(int)WormState::OUTGROUND].PushBack({ 32 * i, 64, 32, 32 });
	animations[(int)WormState::OUTGROUND].loop = false;

	// Hit anim initialize
	for (int i = 0; i < 4; i++) animations[(int)WormState::HIT].PushBack({ 32 * i, 96, 32, 32 });
	animations[(int)WormState::HIT].loop = false;

	// GoDie anim initialize
	for (int i = 0; i < 6; i++) animations[(int)WormState::DIE].PushBack({ 32 * i, 128, 32, 32 });
	animations[(int)WormState::DIE].loop = false;

	// Init general value
	for (int i = 0; i < (int)WormState::MAX; i++)
	{
		animations[i].hasIdle = false;
		animations[i].duration = 0.08f;
	}
}

void Worm::InitStateMachine()
{
	stateMachine.AddState("Idle", 0);
	stateMachine.AddState("InGround", 16);
	stateMachine.AddState("UnderGround", 16);
	stateMachine.AddState("OutGround", 1, 160);
	stateMachine.AddState("Hit", 0);
	stateMachine.AddState("Die", 32);

	stateMachine.ChangeState((int)WormState::IDLE);
}

void Worm::InitPhysics()
{
	// Hit Player or get hit
	b2Filter filter;

	filter.categoryBits = app->physics->ENEMY_LAYER; // Who am I

	filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER; // Who will coll with me

	damageTrigger = new Trigger(position, 8, 8, this, "worm");

	damageTrigger->tag = "Enemy";

	damageTrigger->pBody->body->GetFixtureList()->SetFilterData(filter);

	// Body
	b2Filter filterB;

	filterB.categoryBits = app->physics->ENEMY_LAYER;

	filterB.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER & ~app->physics->PLAYER_LAYER & ~app->physics->PROJECTILE_LAYER;

	pBody = app->physics->CreateRectangle(position, 8, 8, this, b2_dynamicBody, app->physics->ENEMY_LAYER);

	pBody->body->GetFixtureList()->SetFilterData(filterB);
}

void Worm::DoInGround()
{
	SetTriggeeActive(false);

	ResetAllCoolDown();

	stateMachine.ChangeState((int)WormState::INGROUND);

	animations[stateMachine.GetCurrentState()].Reset();

	app->audio->PlayFx(digFX);
}

void Worm::DoOutGround()
{
	SetPosition(playerController->GetPosition());

	ResetAllCoolDown();

	stateMachine.ChangeState((int)WormState::OUTGROUND);

	animations[stateMachine.GetCurrentState()].Reset();

	app->audio->PlayFx(digFX);
}

void Worm::ResetAllCoolDown()
{
	groundCoolDown = 1920;

	unergroundCoolDown = (rand() % 180) * 16;

	shakeOutGround = 480;
}

void Worm::SetTriggeeActive(bool active)
{
	if (pBody->body->IsActive() == active) return;

	damageTrigger->pBody->body->SetActive(active);

	pBody->body->SetActive(active);
}
