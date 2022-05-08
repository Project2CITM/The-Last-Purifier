#include "Bar.h"
#include "ModuleRender.h"

Bar::Bar(iPoint pos, int width, int height, int value, SDL_Color totalColor, SDL_Color currentColor) : GameObject("Bar", "Bar")
{
	totalValue = value;
	currentValue = value;

	totalRectWidth = width;
	totalRectHeight = width;

	currentRectWidth = width;
	currentRectHeight = width;

	totalRect = {pos.x, pos.y, width, height};
	currentRect = { pos.x, pos.y, width, height };

	totalRectColor = totalColor;
	currentRectColor = currentColor;
}

void Bar::SetValue(int value)
{
	this->currentValue = value;

	SetValuePercentage((float)currentValue / (float)totalValue);

}

void Bar::SetValuePercentage(float percentage)
{
	if (percentage > 1.0f) percentage = 1.0f;

	currentRectWidth = totalRectWidth * percentage;

	currentRect.w = currentRectWidth;

}

void Bar::PostUpdate()
{
	app->renderer->AddRectRenderQueue(totalRect, totalRectColor, false, 4, 2.0, 1.0f);
	app->renderer->AddRectRenderQueue(currentRect, currentRectColor, true, 4, 1.0, 1.0f);
}

void Bar::SetBarPosition(iPoint pos)
{
	totalRect.x = pos.x;
	totalRect.y = pos.y;
	currentRect.x = pos.x;
	currentRect.y = pos.y;
}
