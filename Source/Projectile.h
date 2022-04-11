#ifndef _PROJECTILE_
#define _PROJECTILE_

#include "GameObject.h"

class DamageArea;

class Projectile : public GameObject
{
public:
	Projectile(std::string name, iPoint position, fPoint speed, int damage, bool fire = true, bool stun = false, bool isEnemy = false);

	void FireProjectile(fPoint speed);

	void OnCollisionEnter(PhysBody* col) override;

	bool isEnemy = false;
	bool stun = false;
	int damage = 0;
	DamageArea* damageArea;
};
#endif

