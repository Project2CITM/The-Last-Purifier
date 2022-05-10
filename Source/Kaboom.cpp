#include "Kaboom.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "SceneGame.h"
#include "Player.h"
#include "ModuleRender.h"
#include "DamageArea.h"
#include "ParticleAttackKaboom.h"
#include "ModuleAudio.h"

Kaboom::Kaboom(iPoint pos) :Enemy("kaboom")
{
	// Get player pointer
	SceneGame* sceneGame = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	playerController = sceneGame->player->controller;

	// Init general value
	this->position = pos;

	health = 40;

	moveSpeed = 4;

	damage = 15;

	soulsAmount = 14;

	kaboomColors[0] = { 255,253,117,255 };

	kaboomColors[1] = { 65,144,46,255 };

	// Init texture
	InitRenderObjectWithXml("ghoul");

	// Init StateMachine
	InitStateMachine();

	// Init Animation
	InitAnimation();

	// Init physBody 
	InitPhysics();

	
	HitFX = app->audio->LoadFx("Assets/Audio/SFX/Enemies/Ghoul/sfx_enemyHit3.wav");
	idleFX = app->audio->LoadFx("Assets/Audio/SFX/Enemies/Ghoul/sfx_enemyIdle3.wav");
	deadFX = app->audio->LoadFx("Assets/Audio/SFX/Enemies/Kaboom/sfx_kaboomDeath.wav");
}

Kaboom::~Kaboom()
{
	detectTrigger->Destroy();

	damageTrigger->Destroy();

	attack->pendingToDelete = true;
}

void Kaboom::PreUpdate()
{
	kaboomTimer.Update();
	UpdateStates();

	Enemy::PreUpdate();
}

void Kaboom::Update()
{
	stateMachine.Update();

	Enemy::Update();
}

void Kaboom::PostUpdate()
{
	ChangeColor();

	renderObjects[0].flip = GetLinearVelocity().x > 0 ? SDL_FLIP_NONE : GetLinearVelocity().x < 0 ? SDL_FLIP_HORIZONTAL : renderObjects[0].flip;

	animations[stateMachine.GetCurrentState()].Update();

	renderObjects[0].section = animations[stateMachine.GetCurrentState()].GetCurrentFrame();

	Enemy::PostUpdate();
	kaboomTimer.Reset();
}

void Kaboom::Hit(int damage)
{
	stateMachine.ChangeState((int)KaboomState::HIT);

	animations[stateMachine.GetCurrentState()].Reset();

	renderObjects[0].SetColor({ 255,164,164,100 });

	Enemy::Hit(damage);
	app->audio->PlayFx(HitFX);
}

void Kaboom::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (trigger == "EnemyDetectPlayer")
	{
		if (col->gameObject->name == "Player")
		{
			detectPlayer = true;
		}
		return;
	}

	Enemy::OnTriggerEnter(trigger, col);
}

void Kaboom::OnTriggerExit(std::string trigger, PhysBody* col)
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

void Kaboom::Die(bool spawnPower, bool spawnSouls)
{
	stateMachine.ChangeState((int)KaboomState::DIE);
	app->audio->PlayFx(deadFX);
}

void Kaboom::UpdateStates()
{
	SetLinearVelocity(b2Vec2{ 0,0 });

	switch (stateMachine.GetCurrentState())
	{
	case (int)KaboomState::IDLE:
	{
		if(detectPlayer && !playerController->IsInvulnerable() && !playerController->IsDashing()) DoAttack();

		stateMachine.ChangeState((int)KaboomState::RUN);

		DoRun();
		
	}
	break;
	case (int)KaboomState::RUN:
	{
		DoRun();

		// Test codes
		// app->renderer->AddLineRenderQueue(position, player->GetPosition(), false, { 255,255,255,255 }, 2);

		if (detectPlayer && !playerController->IsInvulnerable() && !playerController->IsDashing()) DoAttack();
	}
	break;
	case (int)KaboomState::ATTACK:
	{
		// TODO: No debería reiniciarse esta variable en algún lugar? (attackCoolDown)
		attackCoolDown -= kaboomTimer.getDeltaTime() * 1000;

		// Just can hit a player when animation is attackingsa
		if (attackCoolDown <= 0)
		{
			if (!attack->pBody->body->IsActive())
			{
				damageTrigger->pBody->body->SetActive(false);

				pBody->body->SetActive(false);

				attack->pBody->body->SetActive(true);

				iPoint attackOffset = { -27,-25 };

				new ParticleAttackKaboom(position + attackOffset);
			}
		}
			
		// When finish attack
		if(stateMachine.ChangeState((int)KaboomState::DIE))
		{
			attack->pBody->body->SetActive(false);
		}
	}
	break;
	case (int)KaboomState::HIT:
	{
		if (!animations[stateMachine.GetCurrentState()].HasFinished()) return;

		renderObjects[0].SetColor(kaboomColors[isFirstColor]);

		stateMachine.ChangeState((int)KaboomState::IDLE);
	}
	break;
	case (int)KaboomState::DIE:
	{
		SetTriggeeActive(false);

		if (pBody->body->IsActive()) pBody->body->SetActive(false);

		if (animations[stateMachine.GetCurrentState()].HasFinished()) Enemy::Die(true);
	}
	break;
	}
}

void Kaboom::InitAnimation()
{
	// Create animations

	// Idle anim initialize
	for (int i = 0; i < 4; i++) animations[(int)KaboomState::IDLE].PushBack({ 32 * i, 0, 32, 32 });
	animations[(int)KaboomState::IDLE].loop = true;

	// Run anim initialize
	for (int i = 0; i < 8; i++)animations[(int)KaboomState::RUN].PushBack({ 32 * i, 32, 32, 32 });
	animations[(int)KaboomState::RUN].loop = true;

	// Attack anim initialize
	animations[(int)KaboomState::ATTACK] = animations[(int)KaboomState::IDLE];

	// Hit anim initialize
	for (int i = 0; i < 4; i++)animations[(int)KaboomState::HIT].PushBack({ 32 * i, 96, 32, 32 });
	animations[(int)KaboomState::HIT].loop = false;

	// GoDie anim initialize
	for (int i = 0; i < 6; i++)animations[(int)KaboomState::DIE].PushBack({ 32 * i, 128, 32, 32 });
	animations[(int)KaboomState::DIE].loop = false;

	// Init general value
	for (int i = 0; i < (int)KaboomState::MAX; i++)
	{
		animations[i].hasIdle = false;
		animations[i].duration = 0.0528f;
	}
}

void Kaboom::InitStateMachine()
{
	stateMachine.AddState("Idle", 0);
	stateMachine.AddState("Run", 0);
	stateMachine.AddState("Attack", 4, 720);
	stateMachine.AddState("Hit", 2, 560);
	stateMachine.AddState("Die", 3, 48);

	stateMachine.ChangeState((int)KaboomState::IDLE);
}

void Kaboom::InitPhysics()
{
	// Detect player 
	b2Filter filter;

	filter.categoryBits = app->physics->TRIGGER_LAYER; // Who am I

	filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER; // Who will coll with me

	detectTrigger = new Trigger(position, 16, 12, this, "EnemyDetectPlayer");

	detectTrigger->pBody->body->GetFixtureList()->SetFilterData(filter);

	// Hit Player or get hit
	b2Filter filterB;

	filterB.categoryBits = app->physics->ENEMY_LAYER; // Who am I

	filterB.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER; // Who will coll with me

	damageTrigger = new Trigger(position, 12, this, "ghoul");

	damageTrigger->tag = "Enemy";

	damageTrigger->pBody->body->GetFixtureList()->SetFilterData(filterB);

	// Attack Trigger
	attack = new DamageArea(position, 26, damage);

	attack->pBody->body->SetActive(false);

	attack->pBody->body->GetFixtureList()->SetFilterData(filterB);

	// Body
	b2Filter filterC;

	filterC.categoryBits = app->physics->ENEMY_LAYER;

	filterC.maskBits = app->physics->EVERY_LAYER & ~app->physics->PLAYER_LAYER;

	pBody = app->physics->CreateCircle(position.x, position.y, 12, this, false, b2_dynamicBody, app->physics->ENEMY_LAYER);

	pBody->body->GetFixtureList()->SetFilterData(filterC);
}

void Kaboom::DoAttack()
{
	changeColorTime = 5;

	attack->SetPosition(position);

	stateMachine.ChangeState((int)KaboomState::ATTACK);

	pBody->body->SetActive(false);

	detectTrigger->pBody->body->SetActive(false);

	app->audio->PlayFx(deadFX);
}

void Kaboom::DoRun()
{
	fPoint dir = { (float)(playerController->GetPosition().x - position.x), (float)(playerController->GetPosition().y - position.y) };

	dir = dir.Normalize();

	SetLinearVelocity(b2Vec2{ (float)(dir.x * moveSpeed),(float)(dir.y * moveSpeed) });
	
}

void Kaboom::ChangeColor()
{
	if (stateMachine.GetCurrentState() == (int)KaboomState::HIT) return;

	if (currentColorTime <= 0)
	{
		isFirstColor = !isFirstColor;

		currentColorTime = changeColorTime;

		renderObjects[0].SetColor(kaboomColors[isFirstColor]);
	}

	currentColorTime-= kaboomTimer.getDeltaTime()* 1000;
}

void Kaboom::SetTriggeeActive(bool active)
{
	if (detectTrigger->pBody->body->IsActive() == active) return;

	detectTrigger->pBody->body->SetActive(active);

	damageTrigger->pBody->body->SetActive(active);

	attack->pBody->body->SetActive(active);
}
