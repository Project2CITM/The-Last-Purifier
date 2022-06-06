#include "Ghoul.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "SceneGame.h"
#include "Player.h"
#include "ModuleRender.h"
#include "DamageArea.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "Room.h"
#include "External/Optick/include/optick.h"

Ghoul::Ghoul(iPoint pos, Room* room, bool mut) : Enemy("ghoul"), mutante(mut)
{
	// Get player pointer
	SceneGame* sceneGame = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	playerController = sceneGame->player->controller;

	// Init general value
	this->position = pos;

	this->currentRoom = room;

	if(!mut)
	{
		health = 20;

		moveSpeed = 2;

		damage = 5;
	}
	else
	{
		health = 200;

		moveSpeed = 4;

		damage = 15;
	}
	
	soulsAmount = 10;

	attackCoolDown = 160; // milisegundos

	// Init texture
	InitRenderObjectWithXml("ghoul");

	if (mut)
	{
		renderObjects[0].scale = 2.0f;
		renderObjects[0].screenDetect = false;
	}

	// Init StateMachine
	InitStateMachine();

	// Init Animation
	InitAnimation();

	// Init physBody 
	InitPhysics();

	//Init Sounds
	attackFX = app->audio->LoadFx("Audio/SFX/Enemies/Ghoul/sfx_enemyAttack1.wav");
	HitFX = app->audio->LoadFx("Audio/SFX/Enemies/Ghoul/sfx_enemyHit2.wav");
	idleFX = app->audio->LoadFx("Audio/SFX/Enemies/Ghoul/sfx_enemyIdle1.wav");
}

Ghoul::~Ghoul()
{
	DestroyTriggers();

	attack->pendingToDelete = true;
}

void Ghoul::PreUpdate()
{
	ghoulTimer.Update();

	UpdateStates();

	ghoulTimer.Reset();

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

	renderObjects[0].section = animations[stateMachine.GetCurrentState()].GetCurrentFrame();

	Enemy::PostUpdate();
}

void Ghoul::Hit(int damage)
{
	stateMachine.ChangeState((int)GhoulState::HIT);

	animations[stateMachine.GetCurrentState()].Reset();

	renderObjects[0].SetColor({ 255,164,164,100 });

	Enemy::Hit(damage);
	app->audio->PlayFx(HitFX);
}

void Ghoul::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if(trigger=="EnemyDetectPlayer")
	{
		if (col->gameObject->name == "Player")
		{
			detectPlayer = true;
		}
		return;
	}

	Enemy::OnTriggerEnter(trigger, col);
}

void Ghoul::OnTriggerExit(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (trigger == "EnemyDetectPlayer")
	{
		if (col->gameObject->name == "Player")
		{
			detectPlayer = false;
		}
	}
}

void Ghoul::Die(bool spawnPower, bool spawnSouls)
{
	stateMachine.ChangeState((int)GhoulState::DIE);
}

void Ghoul::UpdateStates()
{
	SetLinearVelocity(b2Vec2{ 0,0 });
	
	switch (stateMachine.GetCurrentState())
	{
	case (int)GhoulState::IDLE:
	{
		if (!detectPlayer)
		{
			ResetAttackCoolDown();

			stateMachine.ChangeState((int)GhoulState::RUN);

			return;			
		}

		if (attackCoolDown <= 0)
		{
			DoAttack();

			ResetAttackCoolDown();

			return;
		}
		attackCoolDown -= ghoulTimer.getDeltaTime() * 1000;
	}
	break;
	case (int)GhoulState::RUN:
	{
		pathFindongCoolDown -= ghoulTimer.getDeltaTime() * 1000;

		DoRun();

		if (pathFindongCoolDown <= 0) pathFindongCoolDown = 200;

		// Test codes
		//app->renderer->AddLineRenderQueue(position, player->GetPosition(), false, { 255,255,255,255 }, 2);

		if(detectPlayer) DoAttack();
	}
	break;
	case (int)GhoulState::ATTACK:
	{
		// Just can hit a player when animation is attacking
		if (animations[stateMachine.GetCurrentState()].getCurrentFrameI() > 2)
		{
			attack->pBody->body->SetActive(true);
		}
		// When finish attack
		if (animations[stateMachine.GetCurrentState()].HasFinished())
		{
			stateMachine.ChangeState((int)GhoulState::IDLE);
			attack->pBody->body->SetActive(false);
		}
	}
		break;
	case (int)GhoulState::HIT:
	{
		if (!animations[stateMachine.GetCurrentState()].HasFinished()) return;
		
		renderObjects[0].SetColor({ 255,255,255,255 });
		
		stateMachine.ChangeState((int)GhoulState::IDLE);
	}	
		break;
	case (int)GhoulState::DIE:
	{
		// Disable Triggers
		// cvSetTriggeeActive(false);

		// Destroy Triggers already!
		DestroyTriggers();

		if (pBody->body->IsActive()) pBody->body->SetActive(false);

		if (animations[stateMachine.GetCurrentState()].HasFinished()) Enemy::Die(true);
	}	
		break;
	}
}

void Ghoul::CleanUp()
{
	currentRoom = nullptr;
}

void Ghoul::InitAnimation()
{
	// Create animations

	// Idle anim initialize
	for (int i = 0; i < 4; i++) animations[(int)GhoulState::IDLE].PushBack({ 32 * i, 0, 32, 32 });
	animations[(int)GhoulState::IDLE].loop = true;

	// Run anim initialize
	for (int i = 0; i < 8; i++) animations[(int)GhoulState::RUN].PushBack({ 32 * i, 32, 32, 32 });
	animations[(int)GhoulState::RUN].loop = true;

	// Attack anim initialize
	for (int i = 0; i < 6; i++) animations[(int)GhoulState::ATTACK].PushBack({ 32 * i, 64, 32, 32 });
	animations[(int)GhoulState::ATTACK].loop = false;

	// Hit anim initialize
	for (int i = 0; i < 4; i++)	animations[(int)GhoulState::HIT].PushBack({ 32 * i, 96, 32, 32 });
	animations[(int)GhoulState::HIT].loop = false;

	// GoDie anim initialize
	for (int i = 0; i < 6; i++) animations[(int)GhoulState::DIE].PushBack({ 32 * i, 128, 32, 32 });
	animations[(int)GhoulState::DIE].loop = false;

	// Init general value
	for (int i = 0; i < (int)GhoulState::MAX; i++)
	{
		animations[i].hasIdle = false;
		animations[i].duration = 0.08f;
	}
}

void Ghoul::InitStateMachine()
{
	stateMachine.AddState("Idle", 0);
	stateMachine.AddState("Run", 0);
	stateMachine.AddState("Attack", 1, 720);
	stateMachine.AddState("Hit", 2, 560);
	stateMachine.AddState("Die", 48);

	stateMachine.ChangeState((int)GhoulState::IDLE);
}

void Ghoul::InitPhysics()
{
	// Detect player 
	b2Filter filter;

	filter.categoryBits = app->physics->TRIGGER_LAYER;

	filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER;
	
	if (mutante) detectTrigger = new Trigger(position, 32, 24, this, "EnemyDetectPlayer");

	else detectTrigger = new Trigger(position, 16, 12, this, "EnemyDetectPlayer");

	detectTrigger->pBody->body->GetFixtureList()->SetFilterData(filter);

	// Hit Player or get hit
	b2Filter filterB;

	filterB.categoryBits = app->physics->ENEMY_LAYER; // Who am I

	filterB.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER; // Who will coll with me

	damageTrigger = new Trigger(position, 12, this, "ghoul");

	damageTrigger->tag = "Enemy";

	damageTrigger->pBody->body->GetFixtureList()->SetFilterData(filterB);

	// Attack Trigger
	if (mutante) attack = new DamageArea(position, 26, 36, damage);
	else attack = new DamageArea(position, 10, 18, damage);

	attack->pBody->body->SetActive(false);

	attack->pBody->body->GetFixtureList()->SetFilterData(filterB);

	// Body
	b2Filter filterC;

	filterC.categoryBits = app->physics->ENEMY_LAYER;

	filterC.maskBits = app->physics->EVERY_LAYER & ~app->physics->PLAYER_LAYER;

	pBody = app->physics->CreateCircle(position.x, position.y, 12, this, false, b2_dynamicBody, app->physics->ENEMY_LAYER);

	pBody->body->GetFixtureList()->SetFilterData(filterC);
}

void Ghoul::DoAttack()
{
	stateMachine.ChangeState((int)GhoulState::ATTACK);

	animations[stateMachine.GetCurrentState()].Reset();

	iPoint attackOffset = { 0,0 };

	if (renderObjects[0].flip == SDL_FLIP_HORIZONTAL) attackOffset = { -15,0 };
	else attackOffset = { 15,0 };

	attack->SetPosition(position + attackOffset);

	app->audio->PlayFx(attackFX);
		
}

void Ghoul::DoRun()
{
	OPTICK_EVENT();

	if (!mutante)
	{
		fPoint fDir = { (float)(playerController->GetPosition().x - position.x), (float)(playerController->GetPosition().y - position.y) };

		fDir = fDir.Normalize();

		lastDir = fDir;

		SetLinearVelocity(b2Vec2{ (float)(fDir.x * moveSpeed),(float)(fDir.y * moveSpeed) });

		return;
	}

	if (currentRoom != nullptr && enable)
	{
		if (pathFindongCoolDown > 0)
		{
			SetLinearVelocity(b2Vec2{ (float)(lastDir.x * moveSpeed),(float)(lastDir.y * moveSpeed) });

			return;
		}

		int distance = position.DistanceTo(playerController->GetPosition());

		if (distance < 50)
		{
			fPoint fDir = { (float)(playerController->GetPosition().x - position.x), (float)(playerController->GetPosition().y - position.y) };

			fDir = fDir.Normalize();

			lastDir = fDir;

			SetLinearVelocity(b2Vec2{ (float)(fDir.x * moveSpeed),(float)(fDir.y * moveSpeed) });

			return;
		}

		iPoint dir = currentRoom->PathFindingAStar(position, playerController->GetPosition());

		if (dir != iPoint{0, 0})
		{
			lastDir.x = dir.x;

			lastDir.y = dir.y;

			SetLinearVelocity(b2Vec2{ (float)(dir.x * moveSpeed),(float)(dir.y * moveSpeed) });
		}
	}
}

void Ghoul::ResetAttackCoolDown()
{
	attackCoolDown = 160;
}

void Ghoul::SetTriggeeActive(bool active)
{
	if (detectTrigger->pBody->body->IsActive() == active) return;

	detectTrigger->pBody->body->SetActive(active);

	damageTrigger->pBody->body->SetActive(active);

	attack->pBody->body->SetActive(active);
}

void Ghoul::DestroyTriggers()
{
	if (detectTrigger != nullptr)
	{
		detectTrigger->Destroy();
		detectTrigger = nullptr;
	}
	if (damageTrigger != nullptr)
	{
		damageTrigger->Destroy();
		damageTrigger = nullptr;
	}
}
