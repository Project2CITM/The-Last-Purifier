#include "SageStaff.h"
#include "ModulePhysics.h"
#include "PlayerController.h"
#include "PlayerCombat.h"
#include "Player.h"

SageStaff::SageStaff(PlayerController* playerController) : SageWeapon(playerController)
{
	attackSpeedCD = 200;

	projectileSpeed = 20;

	projectileDamages[0] = 5;
	projectileDamages[1] = 25;
	projectileDamages[2] = 40;

	chargedAttacksTimes[0] = 100;
	chargedAttacksTimes[1] = 500;
	chargedAttacksTimes[2] = 1800;

	projectileWidths[0] = 2;
	projectileWidths[1] = 4;
	projectileWidths[2] = 8;

	projectileHeights[0] = 2;
	projectileHeights[1] = 4;
	projectileHeights[2] = 8;

}

bool SageStaff::Attack(int chargedTime)
{

	int currentAttack = -1;
	for (int i = 0; i < STAFF_ATTACKS; ++i)
	{
		if (chargedTime < chargedAttacksTimes[i]) break;
		else currentAttack++;
	}

	if (currentAttack == -1) return false;

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

	new Projectile("Projectile", playerController->GetPosition() + attackOffset, duration * projectileSpeed, projectileDamages[currentAttack], particleRotation,
	projectileWidths[currentAttack], projectileHeights[currentAttack]);

    return true;
}

void SageStaff::PreUpdate()
{
}

void SageStaff::CleanUp()
{
}
