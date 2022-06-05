#include "Boss.h"
#include "SceneGame.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "DamageArea.h"
#include "BossProjectile.h"

Boss::Boss(iPoint pos) : Enemy("boss")
{
	// Get player pointer
	SceneGame* sceneGame = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	playerController = sceneGame->player->controller;

	// Init general value
	this->position = pos;

	health = 20;

	moveSpeed = 2;

	damage = 5;

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
	projectile = new  BossProjectile(iPoint(this->position.x - 80, this->position.y - 100));
	projectile2 = new  BossProjectile(iPoint(this->position.x + 50, this->position.y + 60));
}

Boss::~Boss()
{
}

void Boss::PreUpdate()
{
	bossTimer.Update();

	UpdateStates();

	bossTimer.Reset();

	Enemy::PreUpdate();
}

void Boss::Update()
{
	stateMachine.Update();

	Enemy::Update();
}

void Boss::PostUpdate()
{
	renderObjects[0].flip = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	animations[stateMachine.GetCurrentState()].Update();

	renderObjects[0].section = animations[stateMachine.GetCurrentState()].GetCurrentFrame();

	Enemy::PostUpdate();
}

void Boss::Hit(int damage)
{
}

void Boss::OnTriggerEnter(std::string trigger, PhysBody* col)
{
}

void Boss::OnTriggerExit(std::string trigger, PhysBody* col)
{
}

void Boss::Die(bool spawnPower, bool spawnSouls)
{
}

void Boss::UpdateStates()
{
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
		projectile->GoTo(playerController->GetPosition());

		projectile2->GoTo(playerController->GetPosition());
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

	filterB.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER; // Who will coll with me

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
