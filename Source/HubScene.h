#pragma once
#include "SceneGame.h"
#include "HUDInGame.h"
#include "Instructor.h"
#include "PlayerConverter.h"
#include "Quest.h"
#include "NPC.h"

#include "ParticleEmissor.h"

#define HUB_MAP "Main_Hub.tmx"
class Player;
class MiniMap;

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
	
	iPoint playerPos = { 0,0 }; // Use only for ChangePlayer function, NOT THE ACTUAL PLAYER POSITION!!!

	HUDInGame* hudInGame = nullptr;
	MiniMap* miniMap = nullptr;

	iPoint startPosition = { 970, 1485 };

	Instructor* revenantInstructor = nullptr;
	Instructor* sageInstructor = nullptr;

	ClassTree* revenantTree = nullptr;

	PlayerConverter* PlayerChangeClass = nullptr;
	PlayerConverter* PlayerChangeClassBeforeRun = nullptr;

	ParticleEmissor* testEmitter1 = nullptr;

	
public:
	Quest* quest2 = nullptr;
	Quest* quest3 = nullptr;
	Quest* quest4 = nullptr;
	Quest* quest5 = nullptr;

	NPC* npc1 = nullptr;
	NPC* npc2 = nullptr;
	NPC* npc3 = nullptr;
	NPC* npc4 = nullptr;

	bool done1 = false;
	bool done2 = false;
	bool done3 = false;
	bool done4 = false;
};