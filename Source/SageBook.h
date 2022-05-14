#pragma once
#include "SageWeapon.h"
class SageBook : public SageWeapon
{
public:

	SageBook(PlayerController* playerController);

	bool Attack(int chargedTime = 0) override;

	void PreUpdate() override;

	void CleanUp() override;
private:
	void FireProjectile();
private:
	int projectileDamage = 15;

	int timeBetweenProjectiles = 200;
	int currentTimeBetweenProjectiles = 0;

	int numberOfProjectiles = 3;

	bool isAttacking = false;
	int projectilesFired = 0;

};

