#ifndef _PLAYER_CONTROLLER_
#define _PLAYER_CONTROLLER_

#include "GameObject.h"
#include "StateMachine.h"
#include "Animation.h"
#include "Trigger.h"
#include "ModuleEvents.h"


#define PLAYER_ANIMATIONS_NUM 4

enum class PlayerAnim
{
	IDLE = 0,
	RUN,
	ATTACK,
	DASH
};

enum class PlayerState
{
	IDLE = 0,
	RUN,
	ATTACK,
	DASH
};

enum class LookingDirection
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

class Player;
class PlayerCombat;

class PlayerController : public GameObject, public EventListener
{
public:
	PlayerController(std::string name, std::string tag, Player* player);

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	void OnCollisionEnter(PhysBody* col) override;
	void OnCollisionExit(PhysBody* col) override;

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;

	void Hit(int damage);

	void Stun(int frames);

	void Invulnerability(int frames);

	void GameEventTriggered() override;

	bool isVulnerable() { return isInvulnerable; }

public:

	Player* player = nullptr;

	PlayerCombat* combat = nullptr;

	bool canControl = true;

	Animation animations[PLAYER_ANIMATIONS_NUM];

	iPoint textureOffset = { 0,0 };

	bool godMode = false;

	int normalDamage = 0;

	LookingDirection lookingDir = LookingDirection::RIGHT;


private:
	void CreatePhysBody();

	void MovementUpdateKeyboard();
	void MovementUpdateController();

	void CombatUpdate();

	void DashOn();

	fPoint GetPlayerToMouseVector();

	float speed = 12.0f;	//5
	bool isDashing = false;
	int dashCounter = 0;
	int dashTime = 25;
	int dashDistance = 14;	//6

	int invulnerabilityTimeHit = 120;
	bool beenHit = false;

	bool isInvulnerable = false;
	int invulnerabilityCounter = 0;
	int dashInvulnerability = 15;

	PlayerAnim currentAnim = PlayerAnim::IDLE;

	StateMachine stateMachine;
	PlayerState currentState = PlayerState::IDLE;

	Trigger* enemyTrigger = nullptr;

	friend class PlayerCombat;
};

#endif