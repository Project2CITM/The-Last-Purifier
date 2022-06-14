#include "PneumaDamageArea.h"
#include "ModulePhysics.h"
#include "ParticlePneuma.h"

PneumaDamageArea::PneumaDamageArea(iPoint position, int width, int height, int damage) : DamageArea(position, width, height, damage)
{
	// Create Particle Effect
	//new ParticlePneuma(this->GetPosition(), 0, 240);
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