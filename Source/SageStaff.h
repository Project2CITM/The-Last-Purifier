#pragma once
#include "SageWeapon.h"

#define STAFF_ATTACKS 3

class SageChargeBar;

class SageStaff : public SageWeapon
{
public:
	SageStaff(PlayerController* playerController);

	bool Attack(int chargedTime = 0) override;

	void PreUpdate() override;

	void UpdateAttackBar(bool holding, int deltaTimeMS = 0) override;

	void CleanUp() override;
private:

	int chargedAttacksTimes[STAFF_ATTACKS];

	int projectileDamages[STAFF_ATTACKS];

	int projectileWidths[STAFF_ATTACKS];
	int projectileHeights[STAFF_ATTACKS];

	SageChargeBar* sageBar = nullptr;
};

