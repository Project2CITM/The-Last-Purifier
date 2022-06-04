#pragma once
#include "GUI.h"
#include <string>

class Text;

class GUIHoverBox : public GUI
{
private:

	SDL_Color renderColour = { 194, 43, 43, 255 };

	int layer = 4;
	int orderInlayer = 15;

	Text* text = nullptr; 
	Text* costText = nullptr;

	bool controller = false;

public:
	GUIHoverBox();

	~GUIHoverBox();
	
	void Update() override;
	void PostUpdate() override;

	void SetData(std::string txt, int cost = 0, bool controller = false, iPoint position = { 0, 0 });
};

