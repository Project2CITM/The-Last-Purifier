#pragma once
#include "DamageArea.h"
class PneumaDamageArea : public DamageArea
{
public:
	PneumaDamageArea(iPoint position, int damage);

	void PreUpdate() override;

private:
	int totalFrames = 15;
	int currentFrame = 0;
};

