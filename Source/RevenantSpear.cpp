#include "RevenantSpear.h"
#include "ModulePhysics.h"
#include "PlayerController.h"
#include "PlayerCombat.h"
#include "Player.h"
#include "ParticleAttackRevenant.h"

RevenantSpear::RevenantSpear(PlayerController* playerController) : RevenantWeapon(playerController)
{
	// Initialize stats (damage and attackSpeed)

	this->damage = 20;
	this->attackSpeedCD = 240;
	this->playerAttackCD = 300;
	this->attackAreaCD = 80;
	this->nextAttackTime = 200;
	this->maximumAttacks = 3;

	// Create DamageArea with correct filter
	b2Filter filter;

	filter.categoryBits = app->physics->PLAYER_LAYER;

	damageArea = new DamageArea(this->playerController->GetPosition(), 35, 10, damage);

	damageArea->pBody->body->GetFixtureList()->SetFilterData(filter);

	damageArea->pBody->body->SetActive(false);

	weaponTimer = new Timer();
}

bool RevenantSpear::Attack(int chargedTime)
{
	if (!canAttack) return false;

	// Play SFX

	// Set area as active
	damageArea->pBody->body->SetActive(true);

	// Calculate attack offset and rotation based on looking direction
	float attackRotation = 0;
	b2Vec2 attackOffset = this->GetAttackOffset(&attackRotation);

	// Update revenantAttack
	damageArea->damage = this->damage + playerController->player->extraDamage;
	damageArea->stunTime = playerController->player->stunTime;
	damageArea->pushDistance = playerController->player->pushDistance;

	// Place on correct position
	damageArea->pBody->body->SetTransform(playerController->pBody->body->GetPosition() + attackOffset, attackRotation);

	attackAreaActive = true;
	canAttack = false;

	if (currentAttackCounter < maximumAttacks) currentAttackCounter++;
	else currentAttackCounter = 1;

	// Set timer for next attack counter
	nextAttackCounter = nextAttackTime;

	addImpulse = currentAttackCounter == 3;

	iPoint particleOffset;
	int particleRotation = 0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	switch (playerController->lookingDir)
	{
	case LookingDirection::DOWN:
		particleOffset = { -25, -30 };
		particleRotation = 90;
		break;
	case LookingDirection::UP:
		particleOffset = { -25, -20 };
		particleRotation = 270;
		break;
	case LookingDirection::LEFT:
		particleOffset = { -20, -25 };
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case LookingDirection::RIGHT:
		particleOffset = { -30, -25 };
		particleRotation = 0;
		break;

	}

	if (currentAttackCounter == 2) flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);

	new ParticleAttackRevenant(damageArea->GetPosition() + particleOffset, particleRotation, 0.15f, 0, playerController->player->purifiedSwordOn, flip);
	if (currentAttackCounter == 3)
	{
		flip = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);
		new ParticleAttackRevenant(damageArea->GetPosition() + particleOffset, particleRotation, 0.15f, 0, playerController->player->purifiedSwordOn, flip);
	}
	return true;
}

void RevenantSpear::PreUpdate()
{
	addImpulse = false;
	weaponTimer->Update();
	if (!canAttack)
	{
		currentAttackCD += weaponTimer->getDeltaTime() * 1000;
		if (currentAttackCD >= attackSpeedCD)
		{
			canAttack = true;
			currentAttackCD = 0;
		}
	}
	else if (nextAttackCounter > 0)
	{
		nextAttackCounter -= weaponTimer->getDeltaTime() * 1000;
		if (nextAttackCounter <= 0)
		{
			currentAttackCounter = 0;
		}
	}

	if (attackAreaActive)
	{
		currentAttackAreaCD += weaponTimer->getDeltaTime() * 1000;
		if (currentAttackAreaCD >= attackAreaCD)
		{
			attackAreaActive = false;
			damageArea->pBody->body->SetActive(false);
			currentAttackAreaCD = 0;
		}
	}

	weaponTimer->Reset();
}

void RevenantSpear::CleanUp()
{
	damageArea->pendingToDelete = true;
	RELEASE(weaponTimer);
}

b2Vec2 RevenantSpear::GetAttackOffset(float* rotation)
{
	b2Vec2 attackOffset = { 0,0 };
	switch (playerController->lookingDir)
	{
	case LookingDirection::DOWN:
		attackOffset = { 0.5f, 2.25f };
		*rotation = 90 * DEGTORAD;
		break;
	case LookingDirection::UP:
		attackOffset = { 0.5f, -3.0f };
		*rotation = 90 * DEGTORAD;
		break;
	case LookingDirection::LEFT:
		attackOffset = { -2.0f, -0.55f };
		break;
	case LookingDirection::RIGHT:
		attackOffset = { 2.5f, -0.55f };
		break;
	}

	return attackOffset;
}
