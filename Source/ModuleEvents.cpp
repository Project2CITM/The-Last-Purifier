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

void ModuleEvents::RemoveListener(EventListener* listener)
{
	int index = listeners.find(listener);

	if (index >= 0)
	{
		listeners.del(listeners.At(index));
	}
}

void ModuleEvents::TriggerEvent(GameEvent id)
{
	for (int i = 0; i < listeners.count(); i++)
	{
		if (listeners[i] == nullptr) return;
		for (int j = 0; j< MAX_EVENTS_PER_EVENTLISTENER; j++)
		{
			if (listeners[i]->listenTo[j] == id)
			{
				listeners[i]->GameEventTriggered(id);
			}
		}

		
	}
}

bool ModuleEvents::CleanUp()
{
	return true;
}


