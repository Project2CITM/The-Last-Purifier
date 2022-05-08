#ifndef _MODULE_H_
#define _MODULE_H_

#include "Globals.h"
#include "Application.h"

#include "External/PugiXml/src/pugixml.hpp"

//class Application;
class PhysBody;

class Module
{
private :
	bool enabled;

public:
	Application* app = nullptr;

	std::string name = "";

	pugi::xml_node config;

	Module(bool start_enabled = true) : enabled(start_enabled)
	{
		app = Application::GetInstance();
	}

	virtual ~Module()
	{}

	bool IsEnabled() const
	{
		return enabled;
	}

	void Enable()
	{
		if(enabled == false)
		{
			enabled = true;
			Start();
		}
	}

	void Disable()
	{
		if(enabled == true)
		{
			enabled = false;
			CleanUp();
		}
	}

	virtual bool Init(pugi::xml_node&)
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual UpdateStatus PreUpdate()
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	virtual UpdateStatus Update()
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	virtual UpdateStatus PostUpdate()
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	virtual UpdateStatus EndUpdate()
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void GetSaveData(pugi::xml_document& save)
	{
	}

	virtual void LoadSaveData(pugi::xml_document& save)
	{

	}
};

#endif