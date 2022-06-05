#include "PneumaDamageArea.h"
#include "ModulePhysics.h"

PneumaDamageArea::PneumaDamageArea(iPoint position, int damage) : DamageArea(position, 60, 20)
{
	// Create Particle Effect

	currentFrame = totalFrames;

	b2Filter filter;
	filter.categoryBits = app->physics->PLAYER_LAYER;

	this->pBody->body->GetFixtureList()->SetFilterData(filter);
}

void PneumaDamageArea::PreUpdate()
{
	if (currentFrame < 0)return;

	currentFrame--;
	if (currentFrame <= 0) this->pendingToDelete = true;
}
