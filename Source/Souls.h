#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Player.h"

class Trigger;


class Souls : public GameObject
{
public:

	Souls(iPoint position, int soulsAmount);

	~Souls();

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;

private:
	Trigger* trigger = nullptr;

	Trigger* triggerDetectPlayer = nullptr;

	SDL_Texture* soul = nullptr;

	int followDelay = 10;

	int minSpeed = 3;

protected:
	Animation idleAnim;

	Player *player = nullptr;

	bool detectPlayer = false;

	int soulsAmount = 0;
};

