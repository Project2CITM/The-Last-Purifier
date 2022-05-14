#include "ModuleWindow.h"
#include "External/SDL_image/include/SDL_image.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	name = "window";
	window = NULL;
	screenSurface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(pugi::xml_node& config)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	this->config = config;

	pugi::xml_node n = app->saveF.child("game_state").child("settings");

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		// Create window
		// L01: DONE 6: Load all required configurations from config.xml
		Uint32 flags = SDL_WINDOW_SHOWN;
		//bool fullscreen = n.attribute("fullScreen").as_bool(false);
		bool fullscreen = config.child("fullscreen").attribute("value").as_bool(false);
		bool borderless = config.child("borderless").attribute("value").as_bool(false);
		bool resizable = config.child("resizable").attribute("value").as_bool(false);
		bool fullscreen_window = config.child("fullscreen_window").attribute("value").as_bool(false);

		width = config.child("resolution").attribute("width").as_int(640);
		height = config.child("resolution").attribute("height").as_int(480);
		scale = config.child("resolution").attribute("scale").as_int(1);

		//if (fullscreen == true) flags |= SDL_WINDOW_FULLSCREEN;
		if (borderless == true) flags |= SDL_WINDOW_BORDERLESS;
		if (resizable == true) flags |= SDL_WINDOW_RESIZABLE;
		if (fullscreen_window == true) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		window = SDL_CreateWindow(app->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (fullscreen) ToggleFullScreen(fullscreen);
		app->FullScreenDesktop = fullscreen;

		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			// Get window surface
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

	SDL_Surface* iconSurface = IMG_Load("Sprites/UI/logoGame.png");
	
	SetWindowIcon(iconSurface);

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	app->config.child("window").child("fullscreen").attribute("value") = app->FullScreenDesktop;
	app->SaveGame();
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != nullptr)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::ToggleFullScreen(bool fullScreen)
{
	if (fullScreen)
	{
		SDL_SetWindowFullscreen(app->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		app->FullScreenDesktop = true;
	}
	else
	{
		SDL_SetWindowFullscreen(app->window->window, 0);
		app->FullScreenDesktop = false;
	}
}

void ModuleWindow::SetWindowIcon(SDL_Surface* icon)
{
	SDL_SetWindowIcon(window, icon);
}
