#ifndef __KABOOM_H__
#define __KABOOM_H__

#include "Enemy.h"
#include "PlayerController.h"
#include "Timer.h"

class Trigger;
class DamageArea;
class Room;

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
	Kaboom(iPoint pos, Room* room);

	~Kaboom();

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void Hit(int damage) override;

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;

	void OnTriggerExit(std::string trigger, PhysBody* col) override;

	void Die(bool spawnPower, bool spawnSouls, bool spawnOrb = true) override;

	void UpdateStates();

	void CleanUp() override;

	void DisableCollisions() override;

private:

	void InitAnimation();

	void InitStateMachine();

	void InitPhysics();

	void DoAttack();

	void DoRun();

	void ChangeColor();

	void SetTriggeeActive(bool active);

	void DestroyTriggers();

private:

	Animation animations[(int)KaboomState::MAX];

	StateMachine stateMachine;

	LookingDirection lookingDir = LookingDirection::RIGHT;

	PlayerController* playerController = nullptr;

	Trigger* detectTrigger = nullptr;

	Trigger* damageTrigger = nullptr;

	DamageArea* attack = nullptr;

	int attackCoolDown = 240; // milisegundos

	bool detectPlayer = false;

	// Effects

	int changeColorTime = 320; // milisegundos

	int currentColorTime = 320; // milisegundos

	bool isFirstColor = true;

	SDL_Color kaboomColors[2];

	Room* currentRoom = nullptr;

	Timer kaboomTimer;

	uint HitFX = NULL;
	uint idleFX = NULL;
	uint explodeSFX;
};

#endif // !__KABOOM_H__