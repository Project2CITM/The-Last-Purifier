#pragma once
#include "RevenantWeapon.h"
class RevenantSpear : public RevenantWeapon
{
public:
	
	RevenantSpear(PlayerController* playerController);

	bool Attack(int chargedTime = 0) override;

	void PreUpdate() override;

	void CleanUp() override;

private:

	uint playerAttackFX[3];

	b2Vec2 GetAttackOffset(float* rotation);
};

