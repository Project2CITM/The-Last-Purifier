#include "Boss.h"
#include "SceneGame.h"
#include "ModuleScene.h"
#include "ModulePhysics.h"
#include "DamageArea.h"
#include "BossProjectile.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "BossTransition.h"
#include "MenuManager.h"
#include "ModuleEvents.h"

Boss::Boss(iPoint pos) : Enemy("boss")
{
	// Get player pointer
	SceneGame* sceneGame = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	playerController = sceneGame->player->controller;

	// Init general value
	this->position = pos;

	maxHealh = health = 2000;

	moveSpeed = 3.5f;

	damage = 25;

	soulsAmount = 0;

	// Init texture
	InitRenderObjectWithXml("boss");

	// Init texture
	InitRenderObjectWithXml("bossLaser",1);

	renderObjects[1].draw = false;

	renderObjects->screenDetect = false;

	renderObjects[1].textureCenterX = 0;

	renderObjects[1].textureCenterY = 10;

	renderObjects[1].rotCenter = { 2, 16 };

	renderObjects[1].rotation = 90;

	// Init StateMachine
	InitStateMachine();

	// Init Animation
	InitAnimation();

	// Init physBody 
	InitPhysics();

	// Create his proyectiles
	projectile = new  BossProjectile(iPoint(this->position.x - 80, this->position.y - 100), playerController);
	//projectile2 = new  BossProjectile(iPoint(this->position.x + 50, this->position.y + 60), playerController);

	projectile->SetActive(false);

	// Init missile
	for (int i = 0; i < MISSILE_NUM; i++)
	{
		missiles[i] = new BossMissile(playerController);
	}

	// Init HpUI
	bossHp.bg = bossHp.delayHp = bossHp.currentHp = { 50, 320, 500, 15 };

	// Sfxs------------

	for (int i = 0; i < 4; i++)
	{
		std::string attack = "Assets/Audio/SFX/Enemies/Boss/sfx_bossHit" + std::to_string(i + 1) + ".wav";
		hitSFX[i] = app->audio->LoadFx(attack.c_str(), false);
	}

	dieSFX = app->audio->LoadFx("Assets/Audio/SFX/Enemies/Boss/sfx_bossDie.wav", false);
	laserSFX = app->audio->LoadFx("Assets/Audio/SFX/Enemies/Boss/sfx_bossLaserFire.wav", false);
	bossFinalSFX = app->audio->LoadFx("Assets/Audio/Music/mus_boss1Final.wav", false);

	// Transition
	transition = new BossTransition();
}

Boss::~Boss()
{
	// Stop laserSFX in case it is playing on that moment.
	app->audio->StopFX(laserSFX);

	if (transition) RELEASE(transition);
}

void Boss::PreUpdate()
{
	if (isDie || !enable || transition) return;

	bossTimer.Update();

	UpdateStates();

	bossTimer.Reset();

	Enemy::PreUpdate();
}

void Boss::Update()
{
	if(transition)
	{
		if(!transition->transitionStart)
		{
			transition->transitionStart = true;

			app->events->TriggerEvent(GameEvent::STOP_PLAYER_MOVEMENT);

			app->renderer->camera->SetTarget(this);

			// play boss music
			app->audio->PlayMusic("Audio/Music/mus_boss1.ogg", 0, false);

			MenuManager::GetInstance()->canPause = false;
		}

		transition->Update();

		if (transition->transitionEnd)
		{
			RELEASE(transition);

			app->events->TriggerEvent(GameEvent::RESUME_PLAYER_MOVEMENT);

			app->renderer->camera->SetTarget(playerController);

			// Init attack time

			attack1CoolDown = (rand() % 5000 + 5000);  // 5 - 10 s

			attack2CoolDown = (rand() % 10000 + 10000); // 10 - 20 s

			attack3CoolDown = (rand() % 20000 + 15000); // 15 - 35 s

			MenuManager::GetInstance()->canPause = true;
		}

		return;
	}

	if (isDie || !enable) return;

	stateMachine.Update();

	UpdateHpUI();

	Enemy::Update();
}

void Boss::PostUpdate()
{
	if (isDie || !enable) return;

		if (transition) transition->PostUpdate();

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

	// Update laserAnim
	//laserAnim[1].Update();

	//renderObjects[1].section = laserAnim[1].GetCurrentFrame();

	Enemy::PostUpdate();
}

void Boss::Hit(int damage)
{
	if (invulnarable || !enable) return;

	// Play SFX
	int randomNum = rand() % 4;
	app->audio->PlayFx(hitSFX[randomNum]);

	renderObjects[0].SetColor({ 255,164,164,100 });

	hitEffectCount = 20; //ms

	Enemy::Hit(damage);

	float percent = (float)health / (float)maxHealh;

	if (percent <= 0.5f)
	{
		moveSpeed = 7.0f;
		phase2 = true;
	}
	float hp = (bossHp.bg.w * percent);

	if (percent <= 0) hp = 0;

	bossHp.currentHp.w = (int)hp;
}

void Boss::Die(bool spawnPower, bool spawnSoul, bool spawnOrb)
{
	projectile->CleanUp();

	for (int i = 0; i < MISSILE_NUM; i++)
	{
		missiles[i]->CleanUp();
	}

	SetLinearVelocity(b2Vec2{ 0,0 });

	stateMachine.ChangeState((int)BossState::DIE);

	// Stop laserSFX in case it is playing on that moment.
	app->audio->StopFX(laserSFX);
	app->audio->PlayFx(dieSFX);
	app->audio->PlayFx(bossFinalSFX);
	// Fade Out music once the boss is defeated
	Mix_FadeOutMusic(1.0f);

	app->events->TriggerEvent(GameEvent::BOSS_DIES);

}

void Boss::UpdateStates()
{
	switch (stateMachine.GetCurrentState())
	{
	case (int)BossState::IDLE: stateMachine.ChangeState((int)BossState::RUN); break;
	case (int)BossState::ATTACK: DoAttack(); break;
	case (int)BossState::ATTACK2: DoAttack2(); break;
	case (int)BossState::ATTACK3: DoAttack3(); break;
	case (int)BossState::RUN: 	DoRun(); break;
	case (int)BossState::DIE:

		attack->pBody->body->SetActive(false);

		pBody->body->SetActive(false);

		damageTrigger->pBody->body->SetActive(false);

		if (animations[stateMachine.GetCurrentState()].HasFinished()) Enemy::Die(true);

		break;
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

	// Laser anim initialize
	//for (int i = 0; i < 9; i++) laserAnim[0].PushBack({ 0, 100 * i, 300, 100 });

	//laserAnim[0].hasIdle = false;

	//laserAnim[0].loop = false;

	//laserAnim[0].duration = 0.08f;

	//for (int i = 0; i < 6; i++) laserAnim[1].PushBack({ 0, 900 + 100 * i, 600, 100 });

	//laserAnim[1].hasIdle = false;

	//laserAnim[1].loop = true;

	//laserAnim[1].duration = 0.08f;
}

void Boss::InitStateMachine()
{
	stateMachine.AddState("Idle", 0);
	stateMachine.AddState("Run", 0);
	stateMachine.AddState("Attack", 0);
	stateMachine.AddState("Attack2", 0);
	stateMachine.AddState("Attack3", 0);
	stateMachine.AddState("Die", 1);

	stateMachine.ChangeState((int)BossState::IDLE);
}

void Boss::InitPhysics()
{
	// Hit Player or get hit
	b2Filter filterB;

	filterB.categoryBits = app->physics->ENEMY_LAYER; // Who am I

	filterB.maskBits = app->physics->EVERY_LAYER; // Who will coll with me

	damageTrigger = new Trigger(position, 18, this, "boss");

	damageTrigger->tag = "Enemy";

	damageTrigger->pBody->body->GetFixtureList()->SetFilterData(filterB);

	// Laser attack Trigger
	b2Filter filterA;

	filterA.categoryBits = app->physics->ENEMY_LAYER; // Who am I

	filterA.maskBits = app->physics->EVERY_LAYER & ~app->physics->PROJECTILE_LAYER; // Who will coll with me

	attack = new DamageArea(damageTrigger->GetPosition() + iPoint{50, 50}, 4, 240, damage);

	attack->tag = "BossLaser";

	attack->pBody->body->GetFixtureList()->SetFilterData(filterA);

	attack->pBody->body->SetActive(false);

	// Body
	b2Filter filterC;

	filterC.categoryBits = app->physics->ENEMY_LAYER;

	filterC.maskBits = app->physics->EVERY_LAYER & ~app->physics->PLAYER_LAYER;

	pBody = app->physics->CreateCircle(position.x, position.y, 8, this, false, b2_dynamicBody, app->physics->ENEMY_LAYER);

	pBody->body->GetFixtureList()->SetFilterData(filterC);
}

void Boss::DisableCollisions()
{
	pBody->body->SetActive(false);

	if(damage)
	{
		damageTrigger->pBody->body->SetActive(false);

		damageTrigger->enable = false;
	}
	
	if(attack)
	{
		attack->pBody->body->SetActive(false);

		attack->enable = false;
	}

	projectile->SetActive(false);
}

float Boss::GetAttackAngle()
{
	attackPos = playerController->GetPosition();

	float angle = atan2f(attackPos.y - GetPosition().y, attackPos.x - GetPosition().x);

	angle = angle * RADTODEG;

	angle -= 90;

	return angle;
}

b2Vec2 Boss::AttackPivot(float angle)
{
	float offsetDistance = 12;

	b2Vec2 offset = { -offsetDistance * sin(angle * DEGTORAD), offsetDistance * cosf(angle * DEGTORAD) };

	b2Vec2 ret = (pBody->body->GetPosition() + offset);

	return ret;
}

int Boss::AttackRotation(float newAngle)
{
	int ret = 1;

	float currentAngle = renderObjects[1].rotation;

	newAngle -= 90;

	currentAngle -= 180;

	int alpha = newAngle - currentAngle;

	alpha %= 360;
	
	// Check the rotation
	ret = alpha > 0 && alpha < 180 || alpha < -180 ? 1 : -1;

	return ret;
}

void Boss::DoRun()
{
	if (hitEffectCount > 0)
	{
		SetLinearVelocity(fPoint{0,0});

		return;
	}

	iPoint target = playerController->GetPosition();

	fPoint dir = { (float)(target.x - position.x),(float)(target.y - position.y) };

	dir = dir.Normalize();

	SetLinearVelocity(dir * moveSpeed);

	flip = dir.x > 0 ? false : true;

	// Update attack time

	attack1CoolDown -= bossTimer.getDeltaTime() * 1000;

	attack2CoolDown -= bossTimer.getDeltaTime() * 1000;

	attack3CoolDown -= bossTimer.getDeltaTime() * 1000;

	if (attack1CoolDown <= 0)
	{
		stateMachine.ChangeState((int)BossState::ATTACK);

		animations[stateMachine.GetCurrentState()].Reset();

		attack1CoolDown = (rand() % 5000 + 12000); //12-17s
	}
	else if(attack2CoolDown <= 0)
	{
		stateMachine.ChangeState((int)BossState::ATTACK2);

		invulnarable = true;

		animations[stateMachine.GetCurrentState()].Reset();

		for (int i = 0; i < MISSILE_NUM; i++)
		{
			missiles[i]->AttackRequest();
		}
	}
	else if (attack3CoolDown <= 0)
	{
		stateMachine.ChangeState((int)BossState::ATTACK3);

		attack->pBody->body->SetActive(true);

		renderObjects[1].draw = true;

		attack3Count = 10000; // ms
	}
}

// Proyectil
void Boss::DoAttack()
{
	SetLinearVelocity(fPoint{ 0,0 });

	if (animations[stateMachine.GetCurrentState()].HasFinished())
	{
		if (flip) projectile->SetPosition(position + iPoint{ -50, 0});
		else projectile->SetPosition(position + iPoint{ 50, 0 });

		projectile->SetActive(true);

		if (!phase2)projectile->Attack(7);
		else projectile->Attack(10);

		stateMachine.ChangeState((int)BossState::RUN);
	}
}

// Misil
void Boss::DoAttack2()
{
	SetLinearVelocity(fPoint{ 0,0 });

	bool attackEnd = true;

	for (int i = 0; i < MISSILE_NUM; i++)
	{
		if (!missiles[i]->AttackEnd())
		{
			attackEnd = false;
			break;
		}
	}

	if (attackEnd)
	{
		stateMachine.ChangeState((int)BossState::RUN);

		invulnarable = false;

		animations[stateMachine.GetCurrentState()].Reset();

		attack2CoolDown = (rand() % 7000 + 8000); //8-15s
	}
}

// Laser
void Boss::DoAttack3()
{
	SetLinearVelocity(fPoint{ 0,0 });

	attackAngle = GetAttackAngle();

	int attackRotation = AttackRotation(attackAngle);

	//printf("Angle: %f\n", attackAngle);

	float tempAngle =  attack->GetDegreeAngle();

	tempAngle += attackRotation;

	attack->SetRotation(tempAngle, true, AttackPivot(tempAngle));

	renderObjects[1].rotation += attackRotation;

	attack3Count -= bossTimer.getDeltaTime() * 1000;

	if (!laserSound)
	{
		app->audio->PlayFx(laserSFX);
		laserSound = true;
	}

	if (attack3Count <= 0)
	{
		stateMachine.ChangeState((int)BossState::RUN);

		animations[stateMachine.GetCurrentState()].Reset();

		attack->pBody->body->SetActive(false);

		renderObjects[1].draw = false;

		attack3CoolDown = (rand() % 10000 + 10000); //10-20s

		laserSound = false;

		app->audio->StopFX(laserSFX);
	}
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
