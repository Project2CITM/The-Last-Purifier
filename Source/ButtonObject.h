#pragma once
#include "GameObject.h"

class Timer;

class ButtonObject : public GameObject
{
public:
	ButtonObject(iPoint position, int color = 0);

	void PreUpdate() override;

	void PostUpdate() override;

	void CleanUp() override;

	void OnCollisionEnter(PhysBody* col) override;

	void PressButton();

	void ResetButton();

	bool isPressed = false;

	bool revised = false;

private:
	
	int buttonColor = 0;

	SDL_Rect buttonSectionsUp[3] = { {0,0,152,120},{152,0,152,120},{304,0,152,120} };
	SDL_Rect buttonSectionsDown[3] = { {0,120,152,120},{152,120,152,120}, {304,120,152,120} };

	Timer* buttonTimer = nullptr;

	int resetingMS = 200;
	int currentResetMS = 0;

	bool isResetting;
};

