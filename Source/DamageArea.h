#ifndef _DAMAGE_AREA_
#define _DAMAGE_AREA

#include "GameObject.h"

class DamageArea : public GameObject
{
public:
	DamageArea(iPoint position, int width, int height, int damage, int stunTime = 0, int pushDistance = 0);
	DamageArea(iPoint position, float radius, int damage, int stunTime = 0, int pushDistance = 0);

	int GetDamage() {
		return damage;
	}

	// All posible DamageArea effects
	int damage = 0;
	int stunTime = 0;
	int pushDistance = 0;

};
#endif

