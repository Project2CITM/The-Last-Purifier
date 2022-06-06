#ifndef __BossProjectile_H_
#define __BossProjectile_H_

#include "GameObject.h"
#include "Animation.h"
#include "Boss.h"

class Trigger;

class BossProjectile : public Enemy
{
public:

	BossProjectile(iPoint position);

	~BossProjectile();

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	bool GoTo(iPoint target);

	void SetActive(bool active);

private:
	float GetAttackAngle(iPoint target);

	void AttackFinished();

public: 

	bool attacking = false;

private:

	Trigger* attack = nullptr;

	Animation anim;

	fPoint targetDir = { 0,0 };

	float rotation = 0;

	float speed = 0.0f;
};

#endif // !__BossProjectile_H_