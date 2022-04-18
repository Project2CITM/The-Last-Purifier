#ifndef __WORM_H__
#define __WORM_H__

#include "Enemy.h"
#include "PlayerController.h"

class Trigger;

enum class WormState
{
	IDLE = 0,
	INGROUND,
	UNDERGROUND,
	OUTGROUND,
	HIT,
	DIE,
	MAX
};

class Worm : public Enemy
{
public:

	Worm(iPoint pos);

	~Worm();

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void Hit(int damage) override;

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;

	void OnTriggerExit(std::string trigger, PhysBody* col) override;

	void Die(bool spawnPower) override;

private:

	void UpdateStates();

	void InitAnimation();

	void InitStateMachine();

	void InitPhysics();

	void DoInGround();

	void DoOutGround();

	void ResetAllCoolDown();

	void SetTriggeeActive(bool active);

public:

	Animation animations[(int)WormState::MAX];

	StateMachine stateMachine;

	LookingDirection lookingDir = LookingDirection::RIGHT;

	PlayerController* player = nullptr;

	Trigger* damageTrigger = nullptr;

	int groundCoolDown = 120; // frame

	int unergroundCoolDown = 120; // frame

	int shakeOutGround = 30;

	bool animPause = false;
};

#endif // !__WORM_H__