#pragma once
#include "GameObject.h"
#include "NPC.h"
#include "Trigger.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "Text.h"
#include "ModuleAudio.h"
#include "ModuleMap.h"
#include "ModuleScene.h"
#include "SceneGame.h"
#include "Player.h"

class Text;
class Trigger;

class Quest : public GameObject
{
public:
	Quest(std::string name, iPoint position);

	~Quest();

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;

	void OnTriggerExit(std::string trigger, PhysBody* col) override;

private:

	NPC* npc = nullptr;

	struct DATA
	{
		int w = 170;
		int h = 32;
	}npcData;

	Animation idleAnim;

	Trigger* trigger = nullptr;

	iPoint textPosition = { 0,0 };

	uint dialogNPCFX[4];

	Text* text = nullptr;

	pugi::xml_node configDialog;

	int sentenceOrder = 0;

	List<std::string> sentences;

	bool finished = false;

	bool nearNpc = false;

	bool speaking = false;

	bool exterior = true;

	int sentenceNum = 0;

	pugi::xml_node npcNode;
	pugi::xml_node defaultNode;
	pugi::xml_node doneNode;
	SceneGame* scene = nullptr;
public:

	bool speak = false;
	bool active = true;


};
