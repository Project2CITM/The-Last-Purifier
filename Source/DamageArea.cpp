#include "DamageArea.h"
#include "ModulePhysics.h"

DamageArea::DamageArea(iPoint position, int width, int height, int damage, int stunTime, int pushDistance) : GameObject("DamageArea", "DamageArea")
{
	this->damage = damage;
	this->stunTime = stunTime;
	this->pushDistance = pushDistance;

	pBody = app->physics->CreateRectangleSensor(position, width, height, this);
}

DamageArea::DamageArea(iPoint position, float radius, int damage, int stunTime, int pushDistance) : GameObject("DamageArea", "DamageArea")
{
	this->damage = damage;
	this->stunTime = stunTime;
	this->pushDistance = pushDistance;

	pBody = app->physics->CreateCircle(position.x, position.y, radius, this, true);
}
