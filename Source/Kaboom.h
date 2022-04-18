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

	void Die(bool spawnPower) override;

	void UpdateStates();

private:

	void InitAnimation();

	void InitStateMachine();

	void InitPhysics();

	void DoAttack();

	void DoRun();

	void ChangeColor();

private:

	Animation animations[(int)KaboomState::MAX];

	StateMachine stateMachine;

	LookingDirection lookingDir = LookingDirection::RIGHT;

	PlayerController* playerController = nullptr;

	Trigger* detectTrigger = nullptr;

	Trigger* damageTrigger = nullptr;

	DamageArea* attack = nullptr;

	int attackCoolDown = 15; // frame

	bool detectPlayer = false;

	// Effects

	int changeColorTime = 20; // frame

	int currentColorTime = 20; // frame

	bool isFirstColor = true;

	SDL_Color kaboomColors[2];

	
	uint HitFX = NULL;
	uint idleFX = NULL;
	uint deadFX = NULL;
};

#endif // !__KABOOM_H__