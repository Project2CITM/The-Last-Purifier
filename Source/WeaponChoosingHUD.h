#pragma once

#include "GUIButton.h"
#include "List.h"
#include "RevenantWeapon.h"
#include "SageWeapon.h"
#include "Player.h"
#include "Text.h"
#include "Animation.h"

#define REVENANT_WEAPONS 2
#define SAGE_WEAPONS 3

class PlayerCombat;

class WeaponChoosingHUD 
{
public:
	WeaponChoosingHUD(PlayerClass playerClass);

	void Start();

	void PreUpdate();

	void PostUpdate();

	void CleanUp();

private:
	
	void GamepadControl();

private:

	bool displayHUD = false;

	GUIButton* weapon1Btn = nullptr;
	GUIButton* weapon2Btn = nullptr;
	
	RevenantWeaponIDs revenantIDs[REVENANT_WEAPONS];

	SageWeaponIDs sageIDs[SAGE_WEAPONS];

	List<GUIButton*> buttons;

	PlayerCombat* playerCombat = nullptr;

	PlayerClass playerClass;

	Text* text;

	Application* app = nullptr;

	// Gamepad implementation

	int currentOption = 0;

	RenderObject fondoArmas;

	RenderObject Torch1;
	RenderObject Torch2;

	Animation Torch1Anim;
	Animation Torch2Anim;

};

