#include "RevenantSword.h"
#include "PlayerController.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "Player.h"
#include "PlayerCombat.h"
#include "ParticleAttackRevenant.h"
#include "ModuleAudio.h"
#include "Scene.h"
#include "ModuleScene.h"

RevenantSword::RevenantSword(PlayerController* playerController) : RevenantWeapon(playerController)
{
	// Initialize stats (damage and attackSpeed)

	this->damage = 20;
	this->attackSpeedCD = 240;
	this->attackAreaCD = 80;

	// Create DamageArea with correct filter
	b2Filter filter;

	filter.categoryBits = app->physics->PLAYER_LAYER;

	damageArea = new DamageArea(this->playerController->GetPosition(), 15, 20, damage);

	damageArea->pBody->body->GetFixtureList()->SetFilterData(filter);

	damageArea->pBody->body->SetActive(false);

	// Initialize SFXs
	for (int i = 0; i < 5; i++)
	{
		std::string attack = "Assets/Audio/SFX/Player/Melee/sfx_playerMeleeAttack" + std::to_string(i + 1) + ".wav";
		playerAttackFX[i] = app->audio->LoadFx(attack.c_str());
	}

	weaponTimer = new Timer();
}

bool RevenantSword::Attack()
{
	if (!canAttack) return false;

	// Play SFX
	int randomNum = rand() % 5;
	app->audio->PlayFx(playerAttackFX[randomNum]);

	// Set area as active
	damageArea->pBody->body->SetActive(true);

	// Calculate attack offset and rotation based on looking direction
	b2Vec2 attackOffset = playerController->combat->GetAttackOffset();
	float attackRotation = 0;
	if (attackOffset.x == 0.25f) attackRotation = 90 * DEGTORAD;

	// Update revenantAttack
	damageArea->damage = this->damage + playerController->player->extraDamage;
	damageArea->stunTime = playerController->player->stunTime;
	damageArea->pushDistance = playerController->player->pushDistance;

	// Place on correct position
	damageArea->pBody->body->SetTransform(playerController->pBody->body->GetPosition() + attackOffset, attackRotation);

	iPoint particleOffset;
	int particleRotation = 0;
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
		particleRotation = 180;
		break;
	case LookingDirection::RIGHT:
		particleOffset = { -30, -25 };
		particleRotation = 0;
		break;

	}

	new ParticleAttackRevenant(damageArea->GetPosition() + particleOffset, particleRotation, 0.15f, 0, playerController->player->purifiedSwordOn);

	attackAreaActive = true;

	canAttack = false;

	return true;

}

void RevenantSword::PreUpdate()
{
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

void RevenantSword::CleanUp()
{
	damageArea->pendingToDelete = true;
	RELEASE(weaponTimer);
}