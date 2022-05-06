#pragma once
#include "RevenantWeapon.h"
class RevenantSword : public RevenantWeapon
{
public:
	RevenantSword(PlayerController* playerController);

	bool Attack(int chargedTime = 0) override;

	void PreUpdate() override;

	void CleanUp() override;

private:
	uint playerAttackFX[5];
};

