#pragma once
#include "SceneGame.h"
#include "HUDInGame.h"
#include "Instructor.h"
#include "PlayerConverter.h"

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

	void ChangePlayer();
public:

	bool isChangingPlayer = false;

	PlayerClass currentclass;

private:
	
	iPoint playerPos = { 0,0 }; // Use only for ChangePlayer function, NOT THE ACTUAL PLAYER POSITION!!!

	HUDInGame* hudInGame = nullptr;

	iPoint startPosition = { 970, 1485 };

	Instructor* revenantInstructor = nullptr;
	Instructor* sageInstructor = nullptr;

	ClassTree* revenantTree = nullptr;

	PlayerConverter* PlayerChangeClass = nullptr;
	PlayerConverter* PlayerChangeClassBeforeRun = nullptr;
};