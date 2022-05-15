#include "Column.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "Timer.h"

Column::Column(iPoint pos) : Enemy("Column")
{
	this->pBody = app->physics->CreateRectangle(pos, 16, 24, this, b2_kinematicBody);

	renderObjects[0].InitAsTexture(app->textures->Load("Sprites/Enemies/DestroyPot/pot.png", false, false), pos, { 0,0,0,0 }, 2.0f, 1);

	renderObjects[0].textureCenterX = 8;
	renderObjects[0].textureCenterY = 10;

	sections[0] = { 0,0,16,20 };
	sections[1] = { 16,0,16,20 };

	columnTimer = new Timer();
}

Column::~Column()
{
	RELEASE(columnTimer);
}

void Column::PreUpdate()
{
	columnTimer->Update();

	if (destroyed)
	{
		destroyedMS -= columnTimer->getDeltaTime() * 1000;
		if (destroyedMS <= 0) pendingToDelete = true;
	}

	columnTimer->Reset();
}

void Column::PostUpdate()
{
	if (destroyed) renderObjects[0].section = sections[1];
	else renderObjects[0].section = sections[0];

	GameObject::PostUpdate();
}

void Column::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->tag == "SlimeLaser")
	{
		destroyed = true;
	}
}
