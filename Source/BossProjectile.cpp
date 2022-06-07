#include "BossProjectile.h"
#include "Trigger.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "PlayerController.h"

BossProjectile::BossProjectile(iPoint position, PlayerController* target) :Enemy("bossProjectile")
{
	this->position = position;

	playerController = target;

	// Init renderObject
	InitRenderObjectWithXml("bossProjectile");

	renderObjects[0].center = SDL_Point{ 35, 35 };

	//Init anim
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			anim.PushBack({ j * 35,i * 35,35,35 });
		}
	}

	anim.loop = true;

	anim.hasIdle = false;

	anim.duration = 0.2f;

	damage = 20;

	//// Init physic
	b2Filter filterB;

	filterB.categoryBits = app->physics->ENEMY_LAYER; // Who am I

	filterB.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER; // Who will coll with me

	attack = new Trigger(position, 8, this, "bossProjectile");

	attack->tag = "Enemy";

	attack->pBody->body->GetFixtureList()->SetFilterData(filterB);

	// Body
	b2Filter filterC;

	filterC.categoryBits = app->physics->ENEMY_LAYER;

	filterC.maskBits = app->physics->EVERY_LAYER & ~app->physics->PLAYER_LAYER;

	pBody = app->physics->CreateCircle(position.x, position.y, 8, this, true, b2_dynamicBody, app->physics->ENEMY_LAYER);

	pBody->body->GetFixtureList()->SetFilterData(filterC);
}

BossProjectile::~BossProjectile()
{
	//attack->pendingToDelete = true;
}

void BossProjectile::Update()
{
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
	{
		rotation+=2;
	}
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	{
		rotation-=2;
	}
	if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		AttackFinished();
	}

	if(attacking)
	{
		if (speed > 0)
		{
			if (attackDistance < attackStartPos.DistanceTo(GetPosition())) speed -= 1.0f;
			
			else speed -= 0.5f;

			SetLinearVelocity(targetDir * speed);
		}
		else
		{
			SetLinearVelocity(fPoint{ 0,0 });

			if (attackTimes-- <= 0)
			{
				attacking = false;
				SetActive(false);
			}
			else GoToTarget();
		}
	}
}

void BossProjectile::PostUpdate()
{
	anim.Update();

	renderObjects[0].section = anim.GetCurrentFrame();

	GameObject::PostUpdate();
}

void BossProjectile::CleanUp()
{
	attack->pendingToDelete = true;

	pendingToDelete = true;

	playerController = nullptr;
}

bool BossProjectile::Attack(int attackTimes)
{
	if (attacking) return false;

	this->attackTimes = attackTimes;

	attacking = true;

	GoToTarget();

	return true;
}

void BossProjectile::SetActive(bool active)
{
	SetLinearVelocity(fPoint{ 0,0 });

	enable = active;

	pBody->body->SetActive(active);

	attack->enable = active;

	attack->pBody->body->SetActive(active);
}

float BossProjectile::GetAttackAngle(iPoint target)
{
	float angle = atan2f(target.y - GetPosition().y, target.x - GetPosition().x);

	angle = angle * RADTODEG;

	return angle;
}

void BossProjectile::GoToTarget()
{
	iPoint target = playerController->GetPosition();

	attackDistance = target.DistanceTo(GetPosition());

	renderObjects[0].rotation = GetAttackAngle(target);

	attackStartPos = GetPosition();

	fPoint dir = { (float)(target.x - attackStartPos.x),(float)(target.y - attackStartPos.y) };

	targetDir = dir.Normalize();
	
	speed = attackDistance;

	speed = speed > 40 ? 40 : speed;
}

void BossProjectile::AttackFinished()
{
	SetLinearVelocity(fPoint{ 0,0 });

	attacking = false;
}
