#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "Scene.h"
#include "GUIButton.h"


class MainMenu : public Scene
{
public:

	// Constructor
	MainMenu();

	// Destructor
	~MainMenu();

	virtual bool InitScene();

	bool Start() override;

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

	bool CleanUp() override;


private:

	RenderObject fondo;
	SDL_Texture* fondo_;
	GUIButton* PlayBUT;
};

#endif //__MAIN_MENU_H__
