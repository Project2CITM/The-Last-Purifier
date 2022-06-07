#include "Boss.h"
#include "SceneGame.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "DamageArea.h"
#include "BossProjectile.h"
#include "ModuleRender.h"

Boss::Boss(iPoint pos) : Enemy("boss")
{
	// Get player pointer
	SceneGame* sceneGame = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	playerController = sceneGame->player->controller;

	// Init general value
	this->position = pos;

	maxHealh = health = 2000;

	moveSpeed = 2;

	damage = 25;

	soulsAmount = 0;

	// Init texture
	InitRenderObjectWithXml("boss");

	// Init StateMachine
	InitStateMachine();

	// Init Animation
	InitAnimation();

	// Init physBody 
	InitPhysics();

	// Create his proyectiles
	projectile = new  BossProjectile(iPoint(this->position.x - 80, this->position.y - 100), playerController);
	projectile2 = new  BossProjectile(iPoint(this->position.x + 50, this->position.y + 60), playerController);

	projectile->SetActive(false);
	projectile2->SetActive(false);

	// Init HpUI
	bossHp.bg = bossHp.delayHp = bossHp.currentHp = { 50, 320, 500, 15 };
}

Boss::~Boss()
{
}

void Boss::PreUpdate()
{
	if (isDie) return;

	bossTimer.Update();

	UpdateStates();

	bossTimer.Reset();

	Enemy::PreUpdate();
}

void Boss::Update()
{
	if (isDie) return;

	stateMachine.Update();

	UpdateHpUI();

	Enemy::Update();
}

void Boss::PostUpdate()
{
	if (isDie) return;

	if (hitEffectCount > 0)
	{
		hitEffectCount -= bossTimer.getDeltaTime() * 1000;
	}
	else
	{
		renderObjects[0].SetColor({ 255,255,255,255 });
	}

	app->renderer->AddRectRenderQueue(bossHp.bg, bossHp.bgColor, false, 3, 3.5f, 0.0f);

	app->renderer->AddRectRenderQueue(bossHp.delayHp, bossHp.hpDelayColor, true, 3, 2.5f, 0.0f);

	app->renderer->AddRectRenderQueue(bossHp.currentHp, bossHp.hpColor, true, 3, 3.0f, 0.0f);

	renderObjects[0].flip = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	animations[stateMachine.GetCurrentState()].Update();

	renderObjects[0].section = animations[stateMachine.GetCurrentState()].GetCurrentFrame();

	Enemy::PostUpdate();
}

void Boss::Hit(int damage)
{
	renderObjects[0].SetColor({ 255,164,164,100 });

	hitEffectCount = 120; //ms

	Enemy::Hit(damage);

	float percent = (float)health / (float)maxHealh;

	float hp = (bossHp.bg.w * percent);

	bossHp.currentHp.w = (int)hp;
}

void Boss::Die(bool spawnPower, bool spawnSouls)
{
	projectile->CleanUp();

	projectile2->CleanUp();

	SetLinearVelocity(b2Vec2{ 0,0 });

	stateMachine.ChangeState((int)BossState::DIE);
}

void Boss::UpdateStates()
{
	switch (stateMachine.GetCurrentState())
	{
	case (int)BossState::IDLE:
		stateMachine.ChangeState((int)BossState::RUN);
		break;
	case (int)BossState::ATTACK:
		break;
	case (int)BossState::ATTACK2:
		break;
	case (int)BossState::ATTACK3:
		break;
	case (int)BossState::RUN: 	DoRun();
		break;
	case (int)BossState::DIE:

		attack->pBody->body->SetActive(false);

		pBody->body->SetActive(false);

		damageTrigger->pBody->body->SetActive(false);

		if (animations[stateMachine.GetCurrentState()].HasFinished()) Enemy::Die(true);

		break;
	}

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		LaserAttack(true);
	}
	if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		LaserAttack(false);
	}
	if (app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		projectile->SetActive(true);

		projectile->Attack(450);
	}
	if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		projectile->SetActive(false);
	}
}

void Boss::CleanUp()
{
}

void Boss::InitAnimation()
{
	// Create animations

	// Idle anim initialize
	for (int i = 0; i < 4; i++) animations[(int)BossState::IDLE].PushBack({ 100 * i, 0, 100, 100 });
	animations[(int)BossState::IDLE].loop = true;

	// Run anim initialize
	for (int i = 0; i < 8; i++) animations[(int)BossState::RUN].PushBack({ 100 * i, 100, 100, 100 });
	animations[(int)BossState::RUN].loop = true;

	// Attack anim initialize
	for (int i = 0; i < 9; i++) animations[(int)BossState::ATTACK].PushBack({ 100 * i, 200, 100, 100 });
	animations[(int)BossState::ATTACK].loop = false;

	// Attack anim initialize
	for (int i = 0; i < 8; i++) animations[(int)BossState::ATTACK2].PushBack({ 100 * i, 300, 100, 100 });
	animations[(int)BossState::ATTACK2].loop = false;

	// Attack anim initialize
	for (int i = 0; i < 7; i++) animations[(int)BossState::ATTACK3].PushBack({ 100 * i, 500, 100, 100 });
	animations[(int)BossState::ATTACK3].loop = false;

	// GoDie anim initialize
	for (int i = 0, count = 14; i < 2; i++)
	{
		for (int j = 0; j < 10; j++, count--)
		{
			if (count <= 0) break;
			animations[(int)BossState::DIE].PushBack({ 100 * j, 700 + i * 100, 100, 100 });
			animations[(int)BossState::DIE].loop = false;
		}
	}

	// Init general value
	for (int i = 0; i < (int)BossState::MAX; i++)
	{
		animations[i].hasIdle = false;
		animations[i].duration = 0.08f;
	}
}

void Boss::InitStateMachine()
{
	stateMachine.AddState("Idle", 0);
	stateMachine.AddState("Run", 0);
	stateMachine.AddState("Attack", 0, 720);
	stateMachine.AddState("Attack2", 0, 720);
	stateMachine.AddState("Attack3", 0, 720);
	//stateMachine.AddState("Hit", 1, 560);
	stateMachine.AddState("Die", 0);

	stateMachine.ChangeState((int)BossState::IDLE);
}

void Boss::InitPhysics()
{
	// Hit Player or get hit
	b2Filter filterB;

	filterB.categoryBits = app->physics->ENEMY_LAYER; // Who am I

	filterB.maskBits = app->physics->EVERY_LAYER; // Who will coll with me

	damageTrigger = new Trigger(position, 16, this, "boss");

	damageTrigger->tag = "Enemy";

	damageTrigger->pBody->body->GetFixtureList()->SetFilterData(filterB);

	// Attack Trigger
	attack = new DamageArea(damageTrigger->GetPosition() + iPoint{50, 50}, 4, 240, damage);

	attack->tag = "BossLaser";

	attack->pBody->body->GetFixtureList()->SetFilterData(filterB);

	LaserAttack(false);

	// Body
	b2Filter filterC;

	filterC.categoryBits = app->physics->ENEMY_LAYER;

	filterC.maskBits = app->physics->EVERY_LAYER & ~app->physics->PLAYER_LAYER;

	pBody = app->physics->CreateCircle(position.x, position.y, 8, this, false, b2_dynamicBody, app->physics->ENEMY_LAYER);

	pBody->body->GetFixtureList()->SetFilterData(filterC);
}

void Boss::SetTriggeeActive(bool active)
{
}

void Boss::LaserAttack(bool active)
{
	attack->pBody->body->SetActive(active);
}

void Boss::DoRun()
{
	//if (hitEffectCount > 0)
	//{
	//	SetLinearVelocity(fPoint{0,0});

	//	return;
	//}

	//iPoint target = playerController->GetPosition();

	//fPoint dir = { (float)(target.x - position.x),(float)(target.y - position.y) };

	//dir = dir.Normalize();

	//SetLinearVelocity(dir * moveSpeed);

	//flip = dir.x > 0 ? false : true;
}

void Boss::UpdateHpUI()
{
	do
	{
		if (bossHp.delayHp.w < bossHp.currentHp.w) break;

		bossHp.startDelay--;

		if (bossHp.startDelay > 0) break;

		if (bossHp.countDelay <= 0)
		{
			bossHp.delayHp.w--;

			bossHp.countDelay = bossHp.maxCountDelay;
		}
		else
		{
			bossHp.countDelay -= bossHp.delaySpeed;

			if (bossHp.delayHp.w > bossHp.currentHp.w) break;

			bossHp.startDelay = bossHp.MaxStartDelay;
		}

	} while (false);
}
