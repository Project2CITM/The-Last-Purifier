#include "Slime.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "SceneGame.h"
#include "Player.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "DamageArea.h"
#include "ModuleInput.h"

Slime::Slime(iPoint pos) : Enemy("slime")
{
	// Get player pointer
	SceneGame* sceneGame = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	playerController = sceneGame->player->controller;

	// Init general value
	this->position = pos;

	health = 40;

	damage = 10;

	soulsAmount = 14;

	attackCoolDown = 160; // milisegundos

	// Init texture
	InitRenderObjectWithXml("slime");

	renderObjects[0].textureCenterX -= 8;
	renderObjects[0].textureCenterY -= 6;
	renderObjects[0].screenDetect = false;

	// Init her attack
	InitAttackRenderObject();

	// Init StateMachine
	InitStateMachine();

	// Init Animation
	InitAnimation();

	// Init physBody 
	InitPhysics();

	chargeLaserSFX = app->audio->LoadFx("Assets/Audio/SFX/Enemies/sfx_laserCharge.wav", false);
	shootLaserSFX = app->audio->LoadFx("Assets/Audio/SFX/Enemies/sfx_laserShoot.wav", false);
}

Slime::~Slime()
{
	if (damageTrigger != nullptr)
	{
		damageTrigger->Destroy();
		damageTrigger = nullptr;
	}
	if(attack)
	{
		attack->pendingToDelete = true;
	}
}

void Slime::PreUpdate()
{
	if (isDie) return;

	slimeTimer.Update();

	UpdateStates();

	slimeTimer.Reset();

	Enemy::PreUpdate();
}

void Slime::Update()
{
	if (isDie) return;

	stateMachine.Update();

	Enemy::Update();
}

void Slime::PostUpdate()
{
	if (isDie) return;

	animations[stateMachine.GetCurrentState()].Update();

	renderObjects[0].section = animations[stateMachine.GetCurrentState()].GetCurrentFrame();

	//Draw Attack

	//iPoint drawPos = GetDrawPosition(1);

	//float offsetDistance = 120;

	//b2Vec2 offset = { -offsetDistance * sin(attackAngle * DEGTORAD), offsetDistance * cosf(attackAngle * DEGTORAD) };

	//renderObjects[1].destRect.x = drawPos.x + offset.x;

	//renderObjects[1].destRect.y = drawPos.y + offset.y;

	//app->renderer->AddRenderObjectRenderQueue(renderObjects[1]);

	Enemy::PostUpdate();
}

void Slime::Hit(int damage)
{
	stateMachine.ChangeState((int)SlimeState::HIT);

	animations[stateMachine.GetCurrentState()].Reset();

	//renderObjects[0].SetColor({ 255,164,164,100 });

	Enemy::Hit(damage);
}

void Slime::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	Enemy::OnTriggerEnter(trigger, col);
}

void Slime::OnTriggerExit(std::string trigger, PhysBody* col)
{
	Enemy::OnTriggerExit(trigger, col);
}

void Slime::Die(bool spawnPower, bool spawnSouls, bool spawnOrb)
{
	stateMachine.ChangeState((int)SlimeState::DIE);
}

void Slime::UpdateStates()
{
	switch (stateMachine.GetCurrentState())
	{
	case (int)SlimeState::IDLE:
	{
		//printf("Idle\n");

		attackCoolDown -= slimeTimer.getDeltaTime() * 1000;

		if (attackCoolDown > 0) return;
		
		attackCoolDown = 5000;

		animations[(int)SlimeState::ATTACK].Reset();

		stateMachine.ChangeState((int)SlimeState::ATTACK);		
	}
	break;
	case (int)SlimeState::ATTACK:
	{
		if (attackTime == 3000) app->audio->PlayFx(chargeLaserSFX);

		attackTime -= slimeTimer.getDeltaTime() * 1000;

		if(attackTime >= 2500)
		{
			attackPos = playerController->GetPosition();
		}

		if(animations[(int)SlimeState::ATTACK].HasFinished()) 
		{
			// Start attack
			if(!renderObjects[1].draw)
			{
				attackAngle = GetAttackAngle();

				attack->SetRotation(attackAngle, true, AttackPivot(attackAngle));

				renderObjects[1].rotation = attackAngle + 90;

				attack->pBody->body->SetActive(true);

				renderObjects[1].draw = true;

				app->audio->StopFX(chargeLaserSFX);

				app->audio->PlayFx(shootLaserSFX);
			}
		}

		// Finish attack
		if (attackTime > 0) return;
		
		attack->pBody->body->SetActive(false);

		renderObjects[1].draw = false;

		attackTime = 3000;

		stateMachine.ChangeState((int)SlimeState::IDLE);		
	}
	break;
	case (int)SlimeState::HIT:
	{
		hitTime -= slimeTimer.getDeltaTime() * 1000;

		if (hitTime > 0) return;

		hitTime = 5000;

		stateMachine.ChangeState((int)SlimeState::IDLE);
	}
	break;
	case (int)SlimeState::DIE:
	{
		//SetTriggeeActive(false);

		DisableCollisions();

		if (!animations[stateMachine.GetCurrentState()].HasFinished()) return;

		Enemy::Die(false, false, false);
	}
	break;
	}
}

void Slime::DisableCollisions()
{
	if(attack)
	{
		attack->pendingToDelete = true;

		attack->pBody->body->SetActive(false);

		attack = nullptr;
	}

	if (damageTrigger != nullptr)
	{
		damageTrigger->Destroy();
		damageTrigger = nullptr;
	}
}

void Slime::InitAnimation()
{
	// Create animations

	// Idle anim initialize
	for (int i = 0; i < 2; i++)	animations[(int)SlimeState::IDLE].PushBack({ 16 * i, 0, 16, 16 });
	animations[(int)SlimeState::IDLE].loop = true;

	// Attack anim initialize
	for (int i = 0; i < 8; i++)	animations[(int)SlimeState::ATTACK].PushBack({ 16 * i, 0, 16, 16 });
	animations[(int)SlimeState::ATTACK].loop = false;

	// Hit anim initialize
	animations[(int)SlimeState::HIT].PushBack({ 16, 0, 16, 16 });
	animations[(int)SlimeState::HIT].loop = true;

	// GoDie anim initialize
	for (int i = 0; i < 6; i++) animations[(int)SlimeState::DIE].PushBack({ 16 * i, 16, 16, 16 });
	animations[(int)SlimeState::DIE].loop = false;

	// Init general value
	for (int i = 0; i < (int)SlimeState::MAX; i++)
	{
		animations[i].hasIdle = false;
		animations[i].duration = 0.1f;
	}
}

void Slime::InitStateMachine()
{
	stateMachine.AddState("Idle", 0);
	stateMachine.AddState("Attack", 1, 10);
	stateMachine.AddState("Hit", 2, 10);
	stateMachine.AddState("Die", 3);

	stateMachine.ChangeState((int)SlimeState::IDLE);
}

void Slime::InitPhysics()
{
	// Hit Player or get hit
	b2Filter filterB;

	filterB.categoryBits = app->physics->ENEMY_LAYER; // Who am I

	filterB.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER; // Who will coll with me

	damageTrigger = new Trigger(position, 8, this, "slime");

	damageTrigger->tag = "Enemy";

	damageTrigger->pBody->body->GetFixtureList()->SetFilterData(filterB);

	// Attack Trigger
	attack = new DamageArea(position, 4, 240, damage);

	attack->tag = "SlimeLaser";

	attack->pBody->body->SetActive(false);

	attack->pBody->body->GetFixtureList()->SetFilterData(filterB);

	// Body
	b2Filter filterC;

	filterC.categoryBits = app->physics->TRIGGER_LAYER;

	filterC.maskBits = app->physics->EVERY_LAYER & ~app->physics->PLAYER_LAYER & ~app->physics->PROJECTILE_LAYER;

	pBody = app->physics->CreateCircle(position.x, position.y, 8, this, false, b2_dynamicBody, app->physics->ENEMY_LAYER);

	pBody->body->GetFixtureList()->SetFilterData(filterC);
}

void Slime::SetTriggeeActive(bool active)
{
	attack->pBody->body->SetActive(false);

	damageTrigger->pBody->body->SetActive(false);
}

b2Vec2 Slime::AttackPivot(float angle)
{
	float offsetDistance = 12;

	b2Vec2 offset = { -offsetDistance * sin(angle*DEGTORAD), offsetDistance * cosf(angle * DEGTORAD) };

	//printf("Angle¡§:%f  X:%f  Y:%f\n", angle, offset.x, offset.y);

	b2Vec2 ret = (pBody->body->GetPosition() + offset);

	return ret;
}

void Slime::InitAttackRenderObject()
{
	renderObjects[1].section = { 0,32,480,16 };
	renderObjects[1].texture = renderObjects[0].texture;
	renderObjects[1].draw = false;
	renderObjects[1].textureCenterX = -5;
	renderObjects[1].textureCenterY = 4;
	renderObjects[1].rotCenter.x = -12;
	renderObjects[1].rotCenter.y = 16;
	renderObjects[1].screenDetect = false;
}

float Slime::GetAttackAngle()
{
	float angle = atan2f(attackPos.y - GetPosition().y, attackPos.x - GetPosition().x);

	angle = angle * RADTODEG;

	angle -= 90;

	return angle;
}