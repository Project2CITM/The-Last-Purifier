#ifndef __BossProjectile_H_
#define __BossProjectile_H_

#include "GameObject.h"
#include "Animation.h"
#include "Boss.h"

class Trigger;
class PlayerController;

class BossProjectile : public Enemy
{
public:

	BossProjectile(iPoint position, PlayerController* target);

	~BossProjectile();

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	bool Attack(int attackTimes);

	void SetActive(bool active);

private:
	float GetAttackAngle(iPoint target);

	void GoToTarget();

	void AttackFinished();

public: 

	bool attacking = false;

private:

	Trigger* attack = nullptr;

	PlayerController* playerController = nullptr;

	Animation anim;

	fPoint targetDir = { 0,0 };

	iPoint attackStartPos = { 0,0 };

	int attackDistance = 0;

	float rotation = 0;

	float speed = 0.0f;

	int attackTimes = 0;
};

#endif // !__BossProjectile_H_