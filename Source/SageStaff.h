#pragma once
#include "SageWeapon.h"

#define STAFF_ATTACKS 3

class SageStaff : public SageWeapon
{
public:
	SageStaff(PlayerController* playerController);

	bool Attack(int chargedTime = 0) override;

	void PreUpdate() override;

	void CleanUp() override;
private:

	int chargedAttacksTimes[STAFF_ATTACKS];

	int projectileDamages[STAFF_ATTACKS];

	int projectileWidths[STAFF_ATTACKS];
	int projectileHeights[STAFF_ATTACKS];
};

