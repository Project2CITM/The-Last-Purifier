#ifndef __WORM_H__
#define __WORM_H__

#include "Enemy.h"
#include "PlayerController.h"
#include "Timer.h"

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

	PlayerController* playerController = nullptr;

	Trigger* damageTrigger = nullptr;

	int groundCoolDown = 1920; // milisegundos

	int unergroundCoolDown = 1920; // milisegundos

	int shakeOutGround = 480;

	bool animPause = false;

	Timer wormTimer;

	uint HitFX = NULL;
	uint idleFX = NULL;
	uint digFX = NULL;
};

#endif // !__WORM_H__