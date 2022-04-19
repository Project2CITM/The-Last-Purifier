#pragma once
#include "DamageArea.h"
class EkriskiDamageArea : public DamageArea
{
public:
	EkriskiDamageArea(iPoint position, int width, int height, int damage);

	void PreUpdate() override;

private:
	int totalFrames;
	int currentFrame;

};

