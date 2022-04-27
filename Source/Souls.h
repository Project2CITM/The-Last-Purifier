#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Player.h"

class Trigger;


class Souls : public GameObject
{
public:

	Souls(iPoint position);

	~Souls();

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;

private:
	Trigger* trigger = nullptr;
	SDL_Texture* soul = nullptr;
protected:
	Animation idleAnim;
	Player *player = nullptr;
	int soulsAmount = 2;


};

