#ifndef __BOSS_MISSILE_H__
#define __BOSS_MISSILE_H__

#include "Enemy.h"
#include "Animation.h"

class Trigger;
class PlayerController;
class ParticleAttackKaboom;

class BossMissile : Enemy
{
public:
	BossMissile(PlayerController* target);

	~BossMissile();

	void Update();

	void PostUpdate();

	void AttackRequest();

	void CleanUp();

	bool AttackEnd() { return attackEnd; }

private:

	void Attack();

	void InitPhysics();

	void InitAnimation();

private:
	Trigger* attack = nullptr;

	Timer missileTimer;

	PlayerController* playerController;

	Animation anim;

	bool attackRequest = false;

	bool attacking = false;

	bool attackEnd = true;

	iPoint targetPosition = { 0,0 };

	int attackCoolDown = 0; //ms

	int fallSpeed = 3;

	ParticleAttackKaboom* explotion = nullptr;

	uint launchSFX;
	uint explodeSFX;
};

#endif // !__BOSS_MISSILE_H__