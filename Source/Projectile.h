#ifndef _PROJECTILE_
#define _PROJECTILE_

#include "GameObject.h"
#include "Animation.h"

class DamageArea;
class ParticleAttackSage;


class Projectile : public GameObject
{
public:
	Projectile(std::string name, iPoint position, fPoint duration, int damage, int rotation = 0, int width = 4, int height = 4, bool fire = true, bool stun = false, bool isEnemy = false);

	Projectile(iPoint position, int damage, int rotation, fPoint duration);

	void FireProjectile(fPoint duration);

	void OnCollisionEnter(PhysBody* col) override;

	void PostUpdate() override;

public:

	bool isEnemy = false;
	bool stun = false;
	int damage = 0;
	DamageArea* damageArea;
	int rotation = 0;
	Animation anim;

private:
	bool isFoteiros = false;

	iPoint spriteOffset[2];

	uint projectileSFX;
	uint explodeSFX;
};
#endif
