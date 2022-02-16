#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "External/SDL/include/SDL.h"
#include "Module.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	//bool Init();
	bool Init(pugi::xml_node&);
	bool CleanUp();

	void SetTitle(const char* title);

	void ToggleFullScreen(bool fullScreen);

	void SetWindowIcon(SDL_Surface* icon);

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screenSurface = nullptr;
	uint width = 0;
	uint height = 0;
	uint scale = 0;
};

#endif // __ModuleWindow_H__