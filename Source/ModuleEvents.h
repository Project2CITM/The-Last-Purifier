#ifndef _MODULE_EVENTS_
#define _MODULE_EVENTS_

#include "Module.h"
#include "EventListener.h"

class ModuleEvents : public Module
{
public:
	// Constructor
	ModuleEvents(bool start_enabled = true);

	// Destructor
	~ModuleEvents();

	bool Init(pugi::xml_node&);

	void AddListener(EventListener* listener);

	void RemoveListener(EventListener* listener);

	void TriggerEvent(GameEvent id);

	bool CleanUp() override;

private:
	List<EventListener*> listeners;
};

#endif  

