#ifndef __HUD_IN_GAME_H__
#define __HUD_IN_GAME_H__

#include "Scene.h"
#include "Player.h"

class GUIButton;

class HUDInGame : public Scene
{
public:

	//Constructor
	HUDInGame();

	//Destructor
	~HUDInGame();

	virtual bool InitScene();

	bool Start() override;

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

	bool CleanUp() override;

private:

	//Player player;
	SDL_Rect hpRect = {0,0,0,0};
	SDL_Rect miniMap = {0,0,0,0};
	SDL_Rect spell1 = {0,0,0,0};
	SDL_Rect spell2 = {0,0,0,0};
	SDL_Rect spell3 = {0,0,0,0};
	SDL_Rect spell4 = {0,0,0,0};

	SDL_Rect pause = {0,0,0,0};

	iPoint button = { 0,0 };

	GUIButton* Proba = nullptr;

};

#endif //__HUD_IN_GAME_H__