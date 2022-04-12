#include "EventListener.h"

EventListener::EventListener(GameEvent id)
{
	listenTo = id;
}

void EventListener::GameEventTriggered()
{
}
