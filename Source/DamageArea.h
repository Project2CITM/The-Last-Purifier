#ifndef _DAMAGE_AREA_
#define _DAMAGE_AREA

#include "GameObject.h"

class DamageArea : public GameObject
{
public:
	DamageArea(iPoint position, int width, int height, int* damage, bool* stun = nullptr, int* stunTime = nullptr, bool* push = nullptr, int* pushDistance = nullptr);
	DamageArea(iPoint position, int radius, int* damage, bool* stun = nullptr, int* stunTime = nullptr, bool* push = nullptr, int* pushDistance = nullptr);

	// All posible DamageArea effects
	int* damage = nullptr;
	bool* stun = nullptr;
	int* stunTime = nullptr;
	bool* push = nullptr;
	int* pushDistance = nullptr;
};
#endif

