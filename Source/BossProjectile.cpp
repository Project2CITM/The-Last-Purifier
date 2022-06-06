#include "BossProjectile.h"
#include "Trigger.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"

BossProjectile::BossProjectile(iPoint position) :Enemy("bossProjectile")
{
	this->position = position;

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

	filterC.maskBits = app->physics->EVERY_LAYER & app->physics->PLAYER_LAYER;

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
			speed -= 0.6f;

			SetLinearVelocity(targetDir * speed);
		}
		else
		{
			SetLinearVelocity(fPoint{ 0,0 });

			attacking = false;
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
	//attack->pendingToDelete = true;
}

bool BossProjectile::GoTo(iPoint target)
{
	if (attacking) return false;

	attacking = true;

	renderObjects[0].rotation = GetAttackAngle(target);

	fPoint dir = { (float)(target.x - GetPosition().x),(float)(target.y - GetPosition().y) };

	targetDir = dir.Normalize();

	speed = 30;

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

void BossProjectile::AttackFinished()
{
	SetLinearVelocity(fPoint{ 0,0 });

	attacking = false;
}
