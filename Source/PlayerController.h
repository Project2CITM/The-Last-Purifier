#ifndef _PLAYER_CONTROLLER_
#define _PLAYER_CONTROLLER_

#include "GameObject.h"
#include "StateMachine.h"
#include "Animation.h"
#include "PlayerCombat.h"

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

class PlayerController : public GameObject
{
public:
	PlayerController(std::string name, std::string tag, Application* _app, Player* player);

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	Player* player = nullptr;

	PlayerCombat* combat = nullptr;

private:

	void MovementUpdate();

	void CombatUpdate();

	void DashOn();

	fPoint GetPlayerToMouseVector();

	Animation animations[PLAYER_ANIMATIONS_NUM];

	float speed = 5.0f;
	bool isDashing = false;
	int dashCounter = 0;
	int dashTime = 55;
	int dashDistance = 6;

	PlayerAnim currentAnim = PlayerAnim::IDLE;
	PlayerState currentState = PlayerState::IDLE;
	LookingDirection lookingDir = LookingDirection::RIGHT;

	StateMachine stateMachine;
};


#endif
