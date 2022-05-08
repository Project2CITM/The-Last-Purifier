#pragma once

#include "Projectile.h"

class PlayerController;
class Application;

enum class SageWeaponIDs
{
	STAFF,
};

class SageWeapon
{
public:
	SageWeapon(PlayerController* playerController);

	virtual bool Attack(int chargedTime = 0);

	virtual void PreUpdate();

	virtual void UpdateAttackBar(bool holding, int deltaTimeMS = 0);

	virtual void Update();

	virtual void PostUpdate();

	virtual void CleanUp();

public:
	bool canAttack = true;
	int attackSpeedCD = 0;
	int currentAttackCounter = 0; // It tells which attack you are currently on
	bool addImpulse = false;

protected:

	Projectile* projectile = nullptr;
	PlayerController* playerController = nullptr;

	int damage = 0;
	float projectileSpeed = 0;

	int nextAttackTime = 0;
	int nextAttackCounter = 0;

	int chargedAttackTime = 0;

	int maximumAttacks = 3;

	int currentAttackCD = 0;

	Timer* weaponTimer;

	Application* app = nullptr;
};

