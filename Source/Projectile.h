#ifndef _PROJECTILE_
#define _PROJECTILE_

#include "GameObject.h"
#include "Animation.h"

class DamageArea;
class ParticleAttackSage;


class Projectile : public GameObject
{
public:
	Projectile(std::string name, iPoint position, fPoint speed, int damage, bool fire = true, bool stun = false, bool isEnemy = false);

	void FireProjectile(fPoint speed);

	void OnCollisionEnter(PhysBody* col) override;

	void PostUpdate() override;

public:

	bool isEnemy = false;
	bool stun = false;
	int damage = 0;
	DamageArea* damageArea;

	Animation anim;

};
#endif
