#ifndef __HUD_IN_GAME_H__
#define __HUD_IN_GAME_H__

#include "Scene.h"
#include "Player.h"

class GUIButton;
class GUISlider;
class GUICheckbox;
class PlayerCombat;

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

	void GetPlayerCombat(PlayerCombat* playerC);

private:

	//Player player;
	SDL_Rect hpRect = {0,0,0,0};
	SDL_Rect miniMap = {0,0,0,0};
	SDL_Rect spell1 = {0,0,0,0};
	SDL_Rect spell2_1 = {0,0,0,0};
	SDL_Rect spell2_2 = {0,0,0,0};
	SDL_Rect spell3_1 = {0,0,0,0};
	SDL_Rect spell3_2 = {0,0,0,0};
	SDL_Rect spell3_3 = {0,0,0,0};
	SDL_Rect spell4_1 = {0,0,0,0};
	SDL_Rect spell4_2 = {0,0,0,0};
	SDL_Rect spell4_3 = {0,0,0,0};
	SDL_Rect spell4_4 = {0,0,0,0};

	iPoint resumeBUT = { 0,0 };
	iPoint settingsBUT = { 0,0 };
	iPoint controlsBUT = { 0,0 };
	iPoint saveBUT = { 0,0 };
	iPoint loadBUT = { 0,0 };
	iPoint quitBUT = { 0,0 };
	iPoint giveUpBUT = { 0,0 };

	GUIButton* ResumeBUT = nullptr;
	GUIButton* SettingsBUT = nullptr;
	GUIButton* ControlsBUT = nullptr;
	GUIButton* SaveBUT = nullptr;
	GUIButton* LoadBUT = nullptr;
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

	PlayerCombat* player = nullptr;

	uint Hover = NULL;
	uint Press = NULL;

	int ControllerPos = 0;
	int ControllerPosOpY = 0;
	int ControllerPosOpX = 0;
	bool AxisPress = false;

	bool startPause = false;

	int* currentSpell = nullptr;
};

#endif //__HUD_IN_GAME_H__