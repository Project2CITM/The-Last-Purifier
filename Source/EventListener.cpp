#include "Application.h"
#include "ModuleEvents.h"

EventListener::EventListener(GameEvent id)
{
	listenTo = id;
	app = Application::GetInstance();
}

void EventListener::GameEventTriggered()
{
}

EventListener::~EventListener()
{
	
}
