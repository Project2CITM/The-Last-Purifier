#pragma once
#include "GameObject.h"
class Bar : public GameObject
{
public:
	Bar(iPoint pos, int width, int height, int value, SDL_Color totalColor, SDL_Color currentColor);

	virtual void SetValue(int value);

	void SetValuePercentage(float percentage);

	void PostUpdate() override;

	void SetBarPosition(iPoint pos);

public:

	SDL_Color totalRectColor;
	SDL_Color currentRectColor;

	iPoint rectPosition;

	int currentValue;

protected:

	SDL_Rect totalRect;
	SDL_Rect currentRect;

	int totalValue;

	int totalRectWidth;
	int totalRectHeight;

	int currentRectWidth;
	int currentRectHeight;

};

