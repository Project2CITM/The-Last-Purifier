#ifndef __GHOUL_H__
#define __GHOUL_H__

#include "Enemy.h"
#include "PlayerController.h"
#include "Timer.h"

class Trigger;
class DamageArea;

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

	void Die(bool spawnPower) override;

	void UpdateStates();

private:

	void InitAnimation();

	void InitStateMachine();

	void InitPhysics();

	void DoAttack();

	void DoRun();

	void ResetAttackCoolDown();

	void SetTriggeeActive(bool active);

private:

	Animation animations[(int)GhoulState::MAX];

	StateMachine stateMachine;

	LookingDirection lookingDir = LookingDirection::RIGHT;

	PlayerController* playerController = nullptr;

	Trigger* detectTrigger = nullptr;  // saber si jugador esta en rango de ataque

	Trigger* damageTrigger = nullptr; // recibe y hace dany

	DamageArea* attack = nullptr; // ataque al jugador

	bool detectPlayer = false;

	int attackCoolDown = 160;

	int attackFXCoolDown = 0;

	Timer ghoulTimer;

	uint attackFX = NULL;
	uint HitFX = NULL;
	uint idleFX = NULL;
};

#endif // !__GHOUL_H__