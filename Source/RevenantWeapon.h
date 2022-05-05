#pragma once
#include "DamageArea.h"
#include "Timer.h"

class PlayerController;
class Application;

class RevenantWeapon
{
public:
	RevenantWeapon(PlayerController* playerController);

	virtual bool Attack();

	virtual void PreUpdate();

	virtual void Update();

	virtual void PostUpdate();

	virtual void CleanUp();

public:
	bool canAttack = true;
	int attackSpeedCD = 0;
protected:

	DamageArea* damageArea = nullptr;
	PlayerController* playerController = nullptr;

	int damage = 0;

	
	int currentAttackCD = 0;

	int attackAreaCD = 0;
	int currentAttackAreaCD = 0;

	Timer* weaponTimer;

	bool attackAreaActive = false;

	Application* app = nullptr;
};
