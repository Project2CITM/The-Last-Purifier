#ifndef __BOSS_H__

#define __BOSS_H__

#define MISSILE_NUM 25

#include "Enemy.h"
#include "PlayerController.h"
#include "BossMissile.h"

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

struct BossHpGUI
{
	SDL_Rect currentHp = { 0,0,0,0 };
	SDL_Rect delayHp = { 0,0,0,0 };
	SDL_Rect bg = { 0,0,0,0 };
	SDL_Color hpColor = { 255,0,0,255 };
	SDL_Color hpDelayColor = { 168, 162, 50,255 };
	SDL_Color bgColor = { 155,155,155,255 };
	int startDelay = 15;
	int MaxStartDelay = 15;
	int countDelay = 5; //frame
	int maxCountDelay = 5;
	int delaySpeed = 3;
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

	void Die(bool spawnPower, bool spawnSouls) override;

	void UpdateStates();

	void CleanUp() override;

private:

	void InitAnimation();

	void InitStateMachine();

	void InitPhysics();

	void DisableCollisions() override;

	float GetAttackAngle();

	b2Vec2 AttackPivot(float angle);

	int AttackRotation(float newAngle);

	void DoRun();

	void DoAttack();

	void DoAttack2();

	void DoAttack3();

	void UpdateHpUI();

private:

	Animation animations[(int)BossState::MAX];

	iPoint attackPos = { 0,0 };

	float attackAngle = 0;

	int attack3Count = 0; //ms

	StateMachine stateMachine;

	LookingDirection lookingDir = LookingDirection::RIGHT;

	PlayerController* playerController = nullptr;

	Trigger* damageTrigger = nullptr; // recibe y hace dany

	DamageArea* attack = nullptr; // ataque al jugador

	int hitEffectCount = 120; // ms

	bool detectPlayer = false;

	bool flip = false;

	int attack1CoolDown = 0; // ms

	int attack2CoolDown = 0; // ms

	int attack3CoolDown = 0; // ms

	Timer bossTimer;

	float moveSpeed = 2.0f;

	BossProjectile* projectile = nullptr;

	bool invulnarable = false;

	BossHpGUI bossHp;

	int maxHealh = 0;

	BossMissile* missiles[MISSILE_NUM] = { nullptr };
};

#endif // !__BOSS_H__