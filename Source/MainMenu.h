#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "GameObject.h"
#include "Scene.h"
#include "List.h"
#include "GUI.h"
#include <string.h>

class MainMenu : public Scene
{
public:

	// Constructor
	MainMenu(std::string name = "null");

	// Destructor
	~MainMenu();

	virtual bool InitScene();

	virtual bool Start();

	virtual bool PreUpdate();

	virtual bool Update();

	virtual bool PostUpdate();

	virtual bool CleanUp();

	virtual void SetSaveData();

	virtual void LoadSaveData(pugi::xml_node save);

	std::string name = "null";

private:

};

#endif //__MAIN_MENU_H__
