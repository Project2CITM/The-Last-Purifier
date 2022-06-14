#include "EkriskiDamageArea.h"
#include "ParticleExplodeEkriksi.h"
#include "ModulePhysics.h"

EkriskiDamageArea::EkriskiDamageArea(iPoint position, int width, int height, int damage) : DamageArea(position, width, height, damage)
{
	new ParticleExplodeEkriksi(this->GetPosition() - iPoint(60, 80), 1.5f);
	currentFrame = totalFrames;

	b2Filter filter;
	filter.categoryBits = app->physics->PLAYER_LAYER;

	this->pBody->body->GetFixtureList()->SetFilterData(filter);
}

void EkriskiDamageArea::PreUpdate()
{
	if (currentFrame < 0)return;

	currentFrame--;
	if (currentFrame <= 0) this->pendingToDelete = true;
}