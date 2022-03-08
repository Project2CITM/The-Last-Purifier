#pragma once
#include "GameObject.h"
#include "AnimationsManager.h"

#define PLAYER_ANIMATIONS_NUM 3

enum class PlayerAnim
{
	IDLE = 0,
	DASH,
	RUN,
};

enum class LookingDirection
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

class PlayerController : public GameObject
{
public:
	PlayerController(std::string name, std::string tag, Application* _app);

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

private:

	void MovementUpdate();

	void DashOn();

	fPoint GetPlayerToMouseVector();

	Animation animations[PLAYER_ANIMATIONS_NUM];

	float speed = 5.0f;
	bool isDashing = false;
	int dashCounter = 0;
	int dashTime = 55;
	int dashDistance = 10;

	PlayerAnim currentAnim = PlayerAnim::IDLE;
	LookingDirection lookingDir = LookingDirection::RIGHT;

	AnimationsManager animManager;
};

