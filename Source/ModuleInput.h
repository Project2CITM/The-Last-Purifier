#ifndef __MODULE_INPUT_H__
#define __MODULE_INPUT_H__

#include "Module.h"

#define MAX_MOUSE_BUTTONS 5
#define MAX_KEYS 300
#define MAX_CONTROLLERS 1
#define MAX_CONTROLLER_BUTTONS 13

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP,
	KEY_ERROR
};

enum JoystickButtons
{
	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_LEFT_SHOULDER,
	BUTTON_RIGHT_SHOULDER,
	BUTTON_START,
	BUTTON_BACK,
	BUTTON_SELECT,
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(bool start_enabled = true);
	~ModuleInput();

	bool Init(pugi::xml_node&);
	UpdateStatus PreUpdate() override;
	bool CleanUp();

	KEY_STATE GetKey(uint id) const
	{
		return (id < MAX_KEYS) ? keyboard[id] : KEY_ERROR;
	}

	KEY_STATE GetMouseButton(uint id) const
	{
		return (id < MAX_MOUSE_BUTTONS) ? mouse_buttons[id] : KEY_ERROR;
	}

	KEY_STATE GetControllerButton(JoystickButtons id) const
	{
		// Game Controller Input handler
		if (controllerHandles[controllerIndex] == nullptr) return KEY_STATE::KEY_ERROR;
		return joystickButtons[id];
	}

	int GetControllerAxis(SDL_GameControllerAxis id) const
	{
		return SDL_GameControllerGetAxis(controllerHandles[controllerIndex], id);
	}


	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

	bool usingGameController = false;

private:
	KEY_STATE* keyboard = nullptr;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS] = { KEY_STATE::KEY_DOWN };
	int mouse_x = 0;
	int mouse_y = 0;

	// Joysticks Controller variables

	int maxJoysticks = 0;
	int controllerIndex = 0;
	KEY_STATE joystickButtons[MAX_CONTROLLER_BUTTONS] = { KEY_STATE::KEY_DOWN };
		
	SDL_GameControllerButton sdlJoystickButtons[MAX_CONTROLLER_BUTTONS] = { SDL_CONTROLLER_BUTTON_A,SDL_CONTROLLER_BUTTON_B,SDL_CONTROLLER_BUTTON_X,SDL_CONTROLLER_BUTTON_Y,
							SDL_CONTROLLER_BUTTON_DPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_DOWN, SDL_CONTROLLER_BUTTON_DPAD_LEFT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
							SDL_CONTROLLER_BUTTON_LEFTSHOULDER, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, SDL_CONTROLLER_BUTTON_START, SDL_CONTROLLER_BUTTON_BACK, SDL_CONTROLLER_BUTTON_MAX };

	SDL_GameController* controllerHandles[MAX_CONTROLLERS];

	void OpenController();

	void UpdateControllerInput();

};

#endif // !__MODULE_INPUT_H__