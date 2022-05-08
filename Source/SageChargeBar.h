#pragma once
#include "Bar.h"
class SageChargeBar : public Bar
{
public:

	SageChargeBar(iPoint pos, int width, int height, int values[3], SDL_Color totalColor, SDL_Color currentColors[3]);

	void PreUpdate();

	void SetValue(int value) override;

	void ResetValues();

private:

	int totalValues[3];

	SDL_Color currentRectColors[3];

	int currentValueMaximum = 0;
};

