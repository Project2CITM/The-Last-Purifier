#ifndef __BOSS_H__

#define __BOSS_H__

#include "Enemy.h"
#include "PlayerController.h"

class BossProjectile;
class DamageArea;
class Room;

enum class BossState
{
	IDLE = 0,
	RUN,
	ATTACK,
	ATTACK2,
	ATTACK3,
	DIE,
	MAX
};

class Boss : public Enemy
{
public:

	Boss(iPoint pos);

	~Boss();

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void Hit(int damage) override;

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;

	void OnTriggerExit(std::string trigger, PhysBody* col) override;

	void Die(bool spawnPower, bool spawnSouls) override;

	void UpdateStates();

	void CleanUp() override;

private:

	void InitAnimation();

	void InitStateMachine();

	void InitPhysics();

	void SetTriggeeActive(bool active);

	void LaserAttack(bool active = false);

private:

	Animation animations[(int)BossState::MAX];

	StateMachine stateMachine;

	LookingDirection lookingDir = LookingDirection::RIGHT;

	PlayerController* playerController = nullptr;

	Trigger* detectTrigger = nullptr;  // saber si jugador esta en rango de ataque

	Trigger* damageTrigger = nullptr; // recibe y hace dany

	DamageArea* attack = nullptr; // ataque al jugador

	bool detectPlayer = false;

	bool flip = false;

	int attackCoolDown = 160;

	int attackFXCoolDown = 0;

	Timer bossTimer;

	BossProjectile* projectile = nullptr;

	BossProjectile* projectile2 = nullptr;
};

#endif // !__BOSS_H__