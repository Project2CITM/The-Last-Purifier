#pragma once
#include "DamageArea.h"
class PneumaDamageArea : public DamageArea
{
public:
	PneumaDamageArea(iPoint position, int width, int height, int damage);

	void PreUpdate() override;

private:
	int totalFrames = 80;
	int currentFrame = 0;
};

