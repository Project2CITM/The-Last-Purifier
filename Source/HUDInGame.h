#ifndef __HUD_IN_GAME_H__
#define __HUD_IN_GAME_H__

#include "Scene.h"
#include "Player.h"
#include "ModuleEvents.h"
#include "RenderObject.hpp"
#include "Spell.h"
#include "Animation.h"

class GUIButton;
class GUISlider;
class GUICheckbox;
class PlayerCombat;
class Text;

enum class CurrentPauseMenu
{
	Pause,
	Settings,
	Controls
};

struct PlayerHpGUI
{
	SDL_Rect currentHp = { 0,0,0,0 };
	SDL_Rect delayHp = { 0,0,0,0 };
	SDL_Rect bg = { 0,0,0,0 };
	SDL_Color hpColor = { 255,0,0,255 };
	SDL_Color hpDelayColor = { 168, 162, 50,255 };
	SDL_Color bgColor = { 155,155,155,255 };
	int startDelay = 15;
	int MaxStartDelay = 15;
	int countDelay = 5;//frame
	int maxCountDelay = 5;
	int delaySpeed = 3;
};

class HUDInGame : public Scene, public EventListener
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

	void GameEventTriggered(GameEvent id) override;

	void SetPlayerCombat(PlayerCombat* playerC);

	void UpdatePlayerHp();

private:
	void InitializeSlots();
	void InitializeSpellSlotsPositions();
	SDL_Rect GetSpellSection(int slot, bool isDeck);
	std::string GetSpellName(SpellID id);
	void UpdateSpellText();

private:

	//Player player;
	PlayerHpGUI playerHp;
	//SDL_Rect miniMap = {0,0,0,0};

	List<RenderObject> spellSlots;
	List<RenderObject> deckSlots;

	List<SDL_Rect> spellSlotsPositions[4];

	Text* currentSpellText = nullptr;
	Text* currentSpellLevelText = nullptr;

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
	GUICheckbox* VSyncCHK = nullptr;

	CurrentPauseMenu currentPauseMenu;

	PlayerCombat* player = nullptr;

	uint Hover = NULL;
	uint Press = NULL;
	
	Text* text = nullptr;

	Animation SpellAnimSelect;
	Animation SpellAnimNoSelect;
	Animation Torch1Anim;
	Animation Torch2Anim;
	Animation FlameHpAnim;

	int ControllerPos = 0;
	int ControllerPosOpY = 0;
	int ControllerPosOpX = 0;
	bool AxisPress = false;

	bool startPause = false;

	int* currentSpell = nullptr;

	int score = 0;
};

#endif //__HUD_IN_GAME_H__