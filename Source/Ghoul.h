#ifndef __GHOUL_H__
#define __GHOUL_H__

#include "Enemy.h"
#include "PlayerController.h"

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

private:

	void InitAnimation();

private:
	Animation animations[(int)GhoulState::MAX];
	StateMachine stateMachine;
	GhoulState currentState = GhoulState::IDLE;
	LookingDirection lookingDir = LookingDirection::RIGHT;
};

#endif // !__GHOUL_H__