#include "SageBook.h"
#include "ModulePhysics.h"
#include "PlayerController.h"
#include "PlayerCombat.h"

SageBook::SageBook(PlayerController* playerController) : SageWeapon(playerController)
{
    playerAttackCD = 0;
    attackSpeedCD = 1200;

    projectileSpeed = 15;

	weaponTimer = new Timer();
}

bool SageBook::Attack(int chargedTime)
{
	if (!canAttack) return false;
	
	FireProjectile();

	canAttack = false;

	return true;
}

void SageBook::PreUpdate()
{
	weaponTimer->Update();
	if (isAttacking)
	{
		currentTimeBetweenProjectiles += weaponTimer->getDeltaTime() * 1000;
		if (currentTimeBetweenProjectiles >= timeBetweenProjectiles)
		{
			FireProjectile();

			if (projectilesFired == numberOfProjectiles) { isAttacking = false; projectilesFired = 0; }

			currentTimeBetweenProjectiles = 0;
		}
	}

	if (!canAttack)
	{
		currentAttackCD += weaponTimer->getDeltaTime() * 1000;
		if (currentAttackCD >= attackSpeedCD)
		{
			currentAttackCD = 0;
			canAttack = true;
		}
	}

	weaponTimer->Reset();
}


void SageBook::CleanUp()
{
	RELEASE(weaponTimer);
}

void SageBook::FireProjectile()
{
	iPoint attackOffset = { METERS_TO_PIXELS(playerController->combat->GetAttackOffset().x), METERS_TO_PIXELS(playerController->combat->GetAttackOffset().y) };

	// Get projectile speed
	fPoint duration = { 0,0 };
	int particleRotation = 0;
	switch (playerController->lookingDir)
	{
	case LookingDirection::UP:
		duration.y = -1;
		particleRotation = 270;
		break;
	case LookingDirection::DOWN:
		duration.y = 1;
		particleRotation = 90;
		break;
	case LookingDirection::LEFT:
		duration.x = -1;
		particleRotation = -180;
		break;
	case LookingDirection::RIGHT:
		duration.x = 1;
		particleRotation = 0;
		break;
	}

	new Projectile("Projectile", playerController->GetPosition() + attackOffset, duration * projectileSpeed, projectileDamage, particleRotation,
		4, 4);

	projectilesFired++;
	isAttacking = true;
}

