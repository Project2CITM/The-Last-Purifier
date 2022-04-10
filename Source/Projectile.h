#pragma once
#include "GameObject.h"
class Projectile : public GameObject
{
public:
	Projectile(std::string name, iPoint position, fPoint speed, float damage, bool fire = true, bool stun = false, bool isEnemy = false);

	void FireProjectile(fPoint speed);

	void OnCollisionEnter(PhysBody* col) override;

	bool isEnemy = false;
	bool stun = false;
	float damage = 0;
};

