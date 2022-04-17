#pragma once
#include "SceneGame.h"
#include "HUDInGame.h"
#include "Instructor.h"

#define HUB_MAP "Main_Hub.tmx"
class Player;

class HubScene : public SceneGame
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

	iPoint startPosition = { 1017, 1054 }; //{ 969, 1485 };

	Instructor* revenantInstructor = nullptr;

};