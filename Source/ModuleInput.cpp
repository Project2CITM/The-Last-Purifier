#include "ModuleInput.h"
#include "ModuleAudio.h"

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	name = "input";

	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	this->config = config;

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG("SDL_INIT_GAMECONTROLLER could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		OpenController();
		SDL_JoystickEventState(SDL_ENABLE);
	}

	return ret;
}

// Called every draw update
UpdateStatus ModuleInput::PreUpdate()
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			usingGameController = false;
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;

	for (int i = 0; i < MAX_MOUSE_BUTTONS; ++i)
	{
		if (buttons & SDL_BUTTON(i))
		{
			usingGameController = false;
			if (mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	if (controllerHandles[controllerIndex] != nullptr)
	{
		UpdateControllerInput();
		if (GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTX) > 10000 || GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTY) > 10000 ||
			GetControllerAxis(SDL_CONTROLLER_AXIS_RIGHTX) > 10000 || GetControllerAxis(SDL_CONTROLLER_AXIS_RIGHTY) > 10000) usingGameController = true;
	}

	if (usingGameController)
	{
		SDL_ShowCursor(0);
	}
	else
	{
		SDL_ShowCursor(1);
	}
	// Handle X button on window

	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case(SDL_QUIT):// X(Close) Button event handler
		{
			app->ExitGame();
			break;
		}
		case(SDL_CONTROLLERDEVICEADDED):
			OpenController();
			break;
		}
	}

	//	Toggle Global Debug
	//if (GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	//{
	//	app->isDebug = !app->isDebug;
	//}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
void ModuleInput::OpenController()
{
	maxJoysticks = SDL_NumJoysticks();
	controllerIndex = 0;
	for (int JoystickIndex = 0; JoystickIndex < maxJoysticks; ++JoystickIndex)
	{
		if (!SDL_IsGameController(JoystickIndex))
		{
			continue;
		}
		if (controllerIndex >= MAX_CONTROLLERS)
		{
			break;
		}
		controllerHandles[controllerIndex] = SDL_GameControllerOpen(JoystickIndex);
		usingGameController = true;
		break;
	}
}

void ModuleInput::UpdateControllerInput()
{
	if (controllerHandles[controllerIndex] == nullptr) return;

	// Temporary button state (only gives if it's pressed or not)
	bool buttons[MAX_CONTROLLER_BUTTONS];

	for (int i = 0; i < MAX_CONTROLLER_BUTTONS; i++)
	{
		// Get Button State
		buttons[i] = SDL_GameControllerGetButton(controllerHandles[controllerIndex], sdlJoystickButtons[i]);

		if (buttons[i] && i == 9)
		{
			printf("R2\n");
		}
		// If button is pressed
		if (buttons[i] == true)
		{
			usingGameController = true;
			if (joystickButtons[i] == KEY_IDLE)
				joystickButtons[i] = KEY_DOWN; // KEY_DOWN if not pressed on previous frame
			else
				joystickButtons[i] = KEY_REPEAT;// KEY_REPEAT if pressed on previous frame
		}
		else
		{
			if (joystickButtons[i] == KEY_REPEAT || joystickButtons[i] == KEY_DOWN)
				joystickButtons[i] = KEY_UP; // KEY_UP if pressed on previous frame
			else
				joystickButtons[i] = KEY_IDLE; // KEY_IDLE if not pressed on previous frame
		}
	}

	
	
}
