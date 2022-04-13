#include "ModuleEvents.h"

ModuleEvents::ModuleEvents(bool start_enabled) : Module(start_enabled)
{
}

ModuleEvents::~ModuleEvents()
{
}

bool ModuleEvents::Init(pugi::xml_node&)
{
	return true;
}

void ModuleEvents::AddListener(EventListener* listener)
{
	listeners.add(listener);
}

void ModuleEvents::TriggerEvent(GameEvent id)
{
	for (int i = 0; i < listeners.count(); i++)
	{
		if (listeners[i]->listenTo == id)
		{
			listeners[i]->GameEventTriggered();
		}
	}
}

bool ModuleEvents::CleanUp()
{
	return true;
}

