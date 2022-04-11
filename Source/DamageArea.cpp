#include "DamageArea.h"
#include "ModulePhysics.h"

DamageArea::DamageArea(iPoint position, int width, int height, int* damage, bool* stun, int* stunTime, bool* push, int* pushDistance) : GameObject("DamageArea", "DamageArea")
{
	this->damage = damage;
	this->stun = stun;
	this->stunTime = stunTime;
	this->push = push;
	this->pushDistance = pushDistance;

	pBody = app->physics->CreateRectangleSensor(position, width, height, this);
}

DamageArea::DamageArea(iPoint position, int radius, int* damage, bool* stun, int* stunTime, bool* push, int* pushDistance) : GameObject("DamageArea", "DamageArea")
{
	this->damage = damage;
	this->stun = stun;
	this->stunTime = stunTime;
	this->push = push;
	this->pushDistance = pushDistance;

	pBody = app->physics->CreateCircle(position.x, position.y, radius, this, true);
}
