#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GUI.h"
#include "ModuleRender.h"

enum class ButtonState
{
	IDLE,
	FOCUS,
	PRESS_DOWN,
	PRESSED
};

class GUIButton : public GUI
{
private:
	RenderObject* renderObject = nullptr;

	SDL_Rect renderSections[3];

	ButtonState lastState = ButtonState::IDLE;

	ButtonState buttonState = ButtonState::IDLE;

	friend class GUISlider;

	SDL_Color renderColour = { defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a };
	int layer = 3;
	int orderInlayer = 10;

public:
	GUIButton(iPoint pos, int width, int height, MenuButton currentmenu, std::string path);

	GUIButton(iPoint pos, int width, int height, MenuButton currentMenu);

	~GUIButton();

	void Update() override;

	void PostUpdate() override;

	bool navigation = false;

	bool isPressed = false;

	bool doAction = false;

	uint Hover = NULL;
	uint Press = NULL;

public:
	//GETTERS AND SETTERS
	void setRenderColour(SDL_Color colour)
	{
		renderColour = colour;
	};

	void setRenderLayer(int layer, int orderInlayer)
	{
		this->layer = layer;
		this->orderInlayer = orderInlayer;
	};
};

#endif // !__GUIBUTTON_H__