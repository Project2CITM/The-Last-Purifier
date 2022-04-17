#include "Trigger.h"
#include "ModulePhysics.h"

/// <summary>
/// Create Rectangle Sensor attached to a gameobject
/// </summary>
/// <param name="pos"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="parent"></param>
/// <param name="name"></param>
/// <param name="follow"></param>
Trigger::Trigger(iPoint pos, int width, int height, GameObject* parent, std::string name, bool follow) : GameObject(name, name)
{
	pBody = app->physics->CreateRectangleSensor(pos, width, height, this);
	followFather = follow;
	this->parent = parent;
}

/// <summary>
/// Create a circle sensor attached to a game object
/// </summary>
/// <param name="pos"></param>
/// <param name="radius"></param>
/// <param name="parent"></param>
/// <param name="name"></param>
/// <param name="follow"></param>
Trigger::Trigger(iPoint pos, int radius, GameObject* parent, std::string name, bool follow) : GameObject(name, name)
{
	pBody = app->physics->CreateCircle(pos.x, pos.y, radius, this, true);
	followFather = follow;
	this->parent = parent;
}

void Trigger::Update()
{
	if (app->Exiting()) return;

	if (parent == nullptr) return;

	if (parent->pendingToDelete)
	{
		pendingToDelete = true;
		return;
	}

	if (!followFather) return;
	
	SetPosition(parent->GetPosition() + positionOffset);
	
}

void Trigger::OnCollisionEnter(PhysBody* col)
{
	if (pendingToDelete) return;

	if (app->Exiting()) return;

	if (parent) parent->OnTriggerEnter(this->name, col);
}

void Trigger::OnCollisionExit(PhysBody* col)
{
	if (pendingToDelete) return;

	if (app->Exiting()) return;

	if (parent) parent->OnTriggerExit(this->name, col);
}

void Trigger::Destroy()
{
	parent = nullptr;

	pendingToDelete = true;
}
