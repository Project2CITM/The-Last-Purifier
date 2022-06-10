#include "BossMissile.h"
#include "Trigger.h"
#include "ModulePhysics.h"
#include "PlayerController.h"
#include "ModuleRender.h"
#include "ParticleAttackKaboom.h"
#include "ModuleInput.h" // Test

BossMissile::BossMissile(PlayerController* target) :Enemy("bossMissile")
{
	playerController = target;

	damage = 40;

	fallSpeed = 5;

	InitRenderObjectWithXml("bossMissile");

	renderObjects[0].rotCenter = { 35,35 };

	renderObjects[0].rotation = 90;

	InitRenderObjectWithXml("missileWarning", 1);

	renderObjects[1].draw = false;

	InitAnimation();

	InitPhysics();

	position = target->GetPosition();

}

BossMissile::~BossMissile()
{
}

void BossMissile::Update()
{
	missileTimer.Update();

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		AttackRequest();
	}

	if (attackRequest)
	{

		attackCoolDown -= missileTimer.getDeltaTime() * 1000;

		if (attackCoolDown <= 0) Attack();
	}
	else if(attacking)
	{
		position.y += fallSpeed;

		if (position.y >= targetPosition.y) // Explotion
		{
			attacking = false;

			attack->pBody->body->SetActive(true);

			iPoint attackOffset = { -10, -10 };

			explotion = new ParticleAttackKaboom(position + attackOffset);
		}
	}
	else if(explotion)
	{
		if(explotion->pendingToDelete)
		{
			explotion = nullptr;

			attack->pBody->body->SetActive(false);

			attackEnd = true;
		}
	}

	missileTimer.Reset();

	GameObject::Update();
}

void BossMissile::PostUpdate()
{
	if (!attacking) return;

	anim.Update();

	renderObjects[0].section = anim.GetCurrentFrame();

	app->renderer->AddRenderObjectRenderQueue(renderObjects[1]);

	GameObject::PostUpdate();
}

void BossMissile::AttackRequest()
{
	if (!attackEnd) return;

	attackRequest = true;

	attackEnd = false;

	attackCoolDown = rand() % 4000 + 1000; // 1 - 6 second
}

void BossMissile::CleanUp()
{
	pendingToDelete = true;

	attack->pendingToDelete = true;

	playerController = nullptr;
}

void BossMissile::Attack()
{
	attackRequest = false;

	attacking = true;

	targetPosition = playerController->GetPosition() - iPoint{ 8, 6 };

	position = targetPosition + iPoint{ 0, -200 };

	renderObjects[1].destRect.x = targetPosition.x;

	renderObjects[1].destRect.y = targetPosition.y;
}

void BossMissile::InitPhysics()
{
	// Attack Trigger
	b2Filter filterB;

	filterB.categoryBits = app->physics->ENEMY_LAYER; // Who am I

	filterB.maskBits = app->physics->EVERY_LAYER; // Who will coll with me

	attack = new Trigger(position, 20, this, "bossMissile");

	attack->tag = "Enemy";

	attack->positionOffset = { 15, 15 };

	attack->pBody->body->GetFixtureList()->SetFilterData(filterB);

	attack->pBody->body->SetActive(false);
}

void BossMissile::InitAnimation()
{
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
}
