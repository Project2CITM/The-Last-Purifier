#pragma once
#include "GameObject.h"
#include "SageWeapon.h"
#include "RevenantWeapon.h"
#include "Player.h"

class PlayerCombat;
class Text;

struct WeaponInfo
{
	PlayerClass weaponClass;

	RevenantWeaponIDs revenantWeaponID;
	SageWeaponIDs sageWeaponID;

	int weaponLevel = 0;
};

class WeaponObject :public GameObject
{
public:
	WeaponObject(iPoint pos, WeaponInfo info);
	~WeaponObject();
	void PreUpdate() override;

	void OnCollisionEnter(PhysBody* col) override;
	void OnCollisionExit(PhysBody* col) override;

	WeaponInfo weaponInfo;

private:
	void SetRenderObjectFromWeaponInfo();

private:
	bool isPlayerIn = false;
	PlayerCombat* playerCombat = nullptr;
	Text* weaponText = nullptr;
};

