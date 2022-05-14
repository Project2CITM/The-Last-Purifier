#pragma once
#include "GameObject.h"
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

private:
	bool isPressed = false;

	int buttonColor = 0;

	SDL_Rect buttonSectionsUp[3] = { {0,0,152,120},{152,0,152,120},{304,0,152,120} };
	SDL_Rect buttonSectionsDown[3] = { {0,120,152,120},{152,120,152,120}, {304,120,152,120} };
};

