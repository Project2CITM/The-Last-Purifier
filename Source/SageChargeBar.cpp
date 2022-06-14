#include "SageChargeBar.h"

SageChargeBar::SageChargeBar(iPoint pos, int width, int height, int values[3], SDL_Color totalColor, SDL_Color currentColors[3]) : Bar(pos, width, height, values[0], totalColor, currentColors[0])
{
	totalValues[0] = values[0];
	totalValues[1] = values[1];
	totalValues[2] = values[2];

	currentRectColors[0] = currentColors[0];
	currentRectColors[1] = currentColors[1];
	currentRectColors[2] = currentColors[2];

	currentValueMaximum = 0;
}

void SageChargeBar::PreUpdate()
{
	currentRectColor = currentRectColors[currentValueMaximum];
}

void SageChargeBar::SetValue(int value)
{
	this->currentValue = value;

	while (currentValue > totalValues[currentValueMaximum] && currentValueMaximum != 2) currentValueMaximum++;

	totalValue = totalValues[currentValueMaximum];

	SetValuePercentage((float)currentValue / (float)totalValue);
}

void SageChargeBar::ResetValues()
{
	currentValueMaximum = 0;
	this->currentValue = 0;
}