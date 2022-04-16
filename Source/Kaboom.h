#ifndef __KABOOM_H__
#define __KABOOM_H__

#include "Enemy.h"
#include "PlayerController.h"

class Trigger;
class DamageArea;

enum class KaboomState
{
	IDLE = 0,
	RUN,
	ATTACK,
	HIT,
	DIE,
	MAX
};

class Kaboom : public Enemy
{
public:
	Kaboom(iPoint pos);

	~Kaboom();

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void Hit(int damage) override;

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;

	void OnTriggerExit(std::string trigger, PhysBody* col) override;

	void Die() override;

	void UpdateStates();

private:

	void InitAnimation();

	void InitStateMachine();

	void InitPhysics();

	void DoAttack();

	void DoRun();

private:

	Animation animations[(int)KaboomState::MAX];

	StateMachine stateMachine;

	LookingDirection lookingDir = LookingDirection::RIGHT;

	PlayerController* player = nullptr;

	Trigger* detectTrigger = nullptr;

	Trigger* damageTrigger = nullptr;

	DamageArea* attack = nullptr;

	bool detectPlayer = false;
};

#endif // !__KABOOM_H__