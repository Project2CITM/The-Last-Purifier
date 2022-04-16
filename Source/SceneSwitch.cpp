#include "SceneSwitch.h"
#include "ModuleScene.h"
#include "ModulePhysics.h"

SceneSwitch::SceneSwitch(int destination, std::string name, std::string tag) : GameObject(name, tag)
{
	this->destination = destination;
	this->name = name;
	this->tag = tag;
}

SceneSwitch::~SceneSwitch()
{

}

void SceneSwitch::CleanUp()
{

}

void SceneSwitch::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->CompareTag("Player"))
	{
		app->scene->ChangeCurrentSceneRequest(destination);
	}

}

void SceneSwitch::OnCollisionExit(PhysBody* col)
{

}