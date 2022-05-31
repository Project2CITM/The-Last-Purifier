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

	friend class GUISlider;

	SDL_Color renderColour = { defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a };
	int layer = 3;
	int orderInlayer = 10;

public:
	GUIButton(iPoint pos, int width, int height, MenuButton currentmenu, SDL_Texture* texture);

	GUIButton(iPoint pos, int width, int height, MenuButton currentMenu);

	GUIButton(iPoint pos, int width, int height, SDL_Texture* texture);

	~GUIButton();

	void Update() override;

	void PostUpdate() override;

	void PressButton(bool doAction = false);

	void HoverButton();

	bool navigation = false;

	bool isPressed = false;

	bool doAction = false;

	ButtonState buttonState = ButtonState::IDLE;

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
private:
	void UpdateMouse();
};

#endif // !__GUIBUTTON_H__