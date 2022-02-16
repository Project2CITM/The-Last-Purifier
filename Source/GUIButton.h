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
	GUIButton(Application* app, iPoint pos, int width, int height, std::string path);

	GUIButton(Application* app, iPoint pos, int width, int height);

	~GUIButton();

	void Update() override;

	void PostUpdate() override;

	bool navigation = false;

	bool isPressed = false;

	bool doAction = false;
};

#endif // !__GUIBUTTON_H__