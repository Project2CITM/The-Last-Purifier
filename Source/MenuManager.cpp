#include "MenuManager.h"
#include "Application.h"
#include "ModuleInput.h"

MenuManager* MenuManager::instance = nullptr;

MenuManager::MenuManager()
{
	app = Application::GetInstance();
}

MenuManager::~MenuManager()
{
}

MenuManager* MenuManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new MenuManager();
	}
	return instance;
}

void MenuManager::PreUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->GetControllerButton(BUTTON_START) == KEY_DOWN)
	{
		if (canPause) app->TogglePause(!app->isPause);
	}
}

void MenuManager::ReleaseInstance()
{
	delete instance;
	instance = nullptr;
}

