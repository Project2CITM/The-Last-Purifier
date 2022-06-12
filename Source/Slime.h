#ifndef __SLIME_H__
#define __SLIME_H__

#include "Enemy.h"
#include "PlayerController.h"

class Trigger;
class DamageArea;
class PlayerController;

enum class SlimeState
{
	IDLE = 0,
	ATTACK,
	HIT,
	DIE,
	MAX
};

class Slime : public Enemy
{

public:

	Slime(iPoint pos);

	~Slime();

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void Hit(int damage) override;

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;

	void OnTriggerExit(std::string trigger, PhysBody* col) override;

	void Die(bool spawnPower, bool spawnSouls, bool spawnOrb = true) override;

	void UpdateStates();

	void DisableCollisions() override;


private:

	void InitAnimation();

	void InitStateMachine();

	void InitPhysics();

	//void DoAttack();

	//void ResetAttackCoolDown();

	void SetTriggeeActive(bool active);

	b2Vec2 AttackPivot(float angle);

	void InitAttackRenderObject();

	float GetAttackAngle();

private:

	Animation animations[(int)SlimeState::MAX];

	StateMachine stateMachine;

	//LookingDirection lookingDir = LookingDirection::RIGHT;

	PlayerController* playerController = nullptr;

	Trigger* damageTrigger = nullptr; // recibe y hace dany

	DamageArea* attack = nullptr; // ataque al jugador

	float attackAngle = 0;

	int attackCoolDown = 5000; //ms

	int attackTime = 3000;

	int hitTime = 5000;

	iPoint attackPos = { 0,0 };

	Timer slimeTimer;

	uint chargeLaserSFX;
	uint shootLaserSFX;

	//int attackFXCoolDown = 0;

	//// PathFinding variable

	//int pathFindongCoolDown = 200;

	//fPoint lastDir = { 0,0 };

	//// SFX 
	//uint attackFX = NULL;

	//uint HitFX = NULL;

	//uint idleFX = NULL;
};

#endif // !__SLIME_H__