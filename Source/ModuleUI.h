#ifndef __MODULE_UI__
#define __MODULE_UI__

#include <iostream>
#include <vector>
#include <stack>

#include "External/SDL/include/SDL.h"

#include "Module.h"

#define MAX_UI_ITEMS 32

class itemUI
{
public:
	std::vector<int> digitVec;
	float digitScale = 0;
	int totalDigits = 0;

	bool isDynamic = false;
	int lifeFrames = 0;
	iPoint speed = { 0,0 };

	int x = 0;
	int y = 0;
	int layer = 0;
	float orderInLayer = 0;

	void ChangeUI(int num);
};

class ModuleUI : public Module
{
public:

	ModuleUI(Application* app, bool start_enabled = true);

	bool Init(pugi::xml_node&);

	bool Start(); //	Initialize UI system

	UpdateStatus Update();	//	Update current UIs

	UpdateStatus PostUpdate();	//	Print current UIs on screen

	// TODO CleanUp Scene
	bool CleanUp();

	uint CreateUI(int num, int x, int y, float scale = 1.0f, int layer = 2, float orderInLayer = 0.1f, bool isDynamic = false, int lifeFrames = 0, iPoint speed = { 0,0 });	// Creates a new UI item on the position given

	void DestroyUI(uint index);			// Deletes an existing UI based on its index

	SDL_Texture* texture = nullptr;

	itemUI* uiArray[MAX_UI_ITEMS] = { nullptr };

	SDL_Rect numSection[10] = { {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} };
};

#endif

