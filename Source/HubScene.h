#pragma once
#include "Scene.h"
#include "HUDInGame.h"

class PlayerRevenant;

class HubScene : public Scene
{
public:

	HubScene();
	~HubScene();

	bool Start() override;
	bool CleanUp() override;

	bool PreUpdate() override;
	bool Update() override;
	bool PostUpdate() override;

	void AddGUIPause(GUI* gui) override;
	void AddGUIControls(GUI* gui) override;
	void AddGUISettingsP(GUI* gui) override;

private:

	HUDInGame hudInGame;

	PlayerRevenant* player;
};