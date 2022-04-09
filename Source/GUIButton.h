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
};

#endif // !__GUIBUTTON_H__