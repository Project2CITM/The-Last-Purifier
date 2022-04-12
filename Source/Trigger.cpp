#include "Trigger.h"
#include "ModulePhysics.h"

/// <summary>
/// Create Rectangle Sensor attached to a gameobject
/// </summary>
/// <param name="pos"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="father"></param>
/// <param name="name"></param>
/// <param name="follow"></param>
Trigger::Trigger(iPoint pos, int width, int height, GameObject* father, std::string name, bool follow) : GameObject(name, name)
{
	pBody = app->physics->CreateRectangleSensor(pos, width, height, this);
	followFather = follow;
	this->father = father;
}

/// <summary>
/// Create a circle sensor attached to a game object
/// </summary>
/// <param name="pos"></param>
/// <param name="radius"></param>
/// <param name="father"></param>
/// <param name="name"></param>
/// <param name="follow"></param>
Trigger::Trigger(iPoint pos, int radius, GameObject* father, std::string name, bool follow) : GameObject(name, name)
{
	pBody = app->physics->CreateCircle(pos.x, pos.y, radius, this, true);
	followFather = follow;
	this->father = father;
}

void Trigger::Update()
{
	if (!followFather ||father == nullptr || father->pendingToDelete) return;

	SetPosition(father->GetPosition() + positionOffset);
	if (father->pendingToDelete) pendingToDelete = true;

}

void Trigger::OnCollisionEnter(PhysBody* col)
{
	father->OnTriggerEnter(this->name, col);
}

void Trigger::OnCollisionExit(PhysBody* col)
{
	father->OnTriggerExit(this->name, col);
}
