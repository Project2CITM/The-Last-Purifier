#pragma once
#include "Scene.h"
#include "HUDInGame.h"

#define HUB_MAP "Main_Hub.tmx"
class Player;

class HubScene : public Scene
{
public:

	HubScene();
	~HubScene();

	bool InitScene() override;
	bool Start() override;
	bool CleanUp() override;

	bool PreUpdate() override;
	bool Update() override;
	bool PostUpdate() override;

	void AddGUIPause(GUI* gui) override;
	void AddGUIControls(GUI* gui) override;
	void AddGUISettingsP(GUI* gui) override;

private:

	HUDInGame* hudInGame = nullptr;

	Player* player;
	iPoint startPosition = { 926, 1579 };
};