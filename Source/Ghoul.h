#ifndef __GHOUL_H__
#define __GHOUL_H__

#include "Enemy.h"
#include "PlayerController.h"

class Trigger;

enum class GhoulState
{
	IDLE = 0,
	RUN,
	ATTACK,
	HIT,
	DIE,
	MAX
};

class Ghoul : public Enemy
{
public:
	Ghoul(iPoint pos);

	~Ghoul();

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

private:

	Animation animations[(int)GhoulState::MAX];

	StateMachine stateMachine;

	LookingDirection lookingDir = LookingDirection::RIGHT;

	PlayerController* player = nullptr;

	Trigger* detectTrigger;

	bool detectPlayer = false;
};

#endif // !__GHOUL_H__