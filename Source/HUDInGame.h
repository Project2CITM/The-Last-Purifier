#ifndef __HUD_IN_GAME_H__
#define __HUD_IN_GAME_H__

#include "Scene.h"
#include "Player.h"

class GUIButton;
class GUISlider;
class GUICheckbox;

enum class CurrentPauseMenu
{
	Pause,
	Settings,
	Controls
};

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

	iPoint resumeBUT = { 0,0 };
	iPoint settingsBUT = { 0,0 };
	iPoint controlsBUT = { 0,0 };
	iPoint quitBUT = { 0,0 };
	iPoint giveUpBUT = { 0,0 };

	GUIButton* ResumeBUT = nullptr;
	GUIButton* SettingsBUT = nullptr;
	GUIButton* ControlsBUT = nullptr;
	GUIButton* GiveUpBUT = nullptr;
	GUIButton* QuitBUT = nullptr;

	GUIButton* CloseControlsBUT = nullptr;

	GUIButton* CloseSettingsBUT = nullptr;
	GUIButton* MusicBUT = nullptr;
	GUIButton* fxBUT = nullptr;

	GUISlider* MusicSlider = nullptr;
	GUISlider* fxSlider = nullptr;

	GUICheckbox* FullScreenCHK = nullptr;

	CurrentPauseMenu currentPauseMenu;

	bool startPause = false;
};

#endif //__HUD_IN_GAME_H__