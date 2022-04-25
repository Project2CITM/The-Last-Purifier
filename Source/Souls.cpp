#include "Souls.h"
#include "Trigger.h"
#include "Application.h"
#include "ModulePhysics.h"

Souls::Souls(iPoint position):GameObject("Souls", "Souls")
{
	this->position = position;
	trigger = new Trigger(position, 50, this, "SoulsTrigger", false);
	InitRenderObjectWithXml("Soul");

}

Souls::~Souls()
{
}

void Souls::Start()
{
}

void Souls::PreUpdate()
{
}

void Souls::Update()
{
}

void Souls::PostUpdate()
{
}

void Souls::CleanUp()
{
}

void Souls::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->name == "Player")
	{
		LOG("Enter");
		//Colocar velocidad hacia jugador

	}

}

void Souls::OnTriggerExit(std::string trigger, PhysBody* col)
{
}
