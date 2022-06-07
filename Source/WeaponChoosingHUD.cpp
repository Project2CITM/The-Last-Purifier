#include "WeaponChoosingHUD.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleEvents.h"
#include "SceneGame.h"
#include "PlayerCombat.h"
#include "PlayerController.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "MenuManager.h"

WeaponChoosingHUD::WeaponChoosingHUD(PlayerClass playerClass)
{
	this->app = Application::GetInstance();

	SceneGame* scene = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	this->playerCombat = scene->player->controller->combat;

	this->playerClass = playerClass;

	std::string weapon1AssetPath = "";
	std::string weapon2AssetPath = "";

	int width1 = 0;
	int height1 = 0;
	int width2 = 0;
	int height2 = 0;

	iPoint pos1 = { 0,0 };
	iPoint pos2 = { 0,0 };
	iPoint posfondo = { 40,10 };

	if (playerClass == PlayerClass::REVENANT)
	{
		// Set up variables
		revenantIDs[0] = RevenantWeaponIDs::SWORD;
		revenantIDs[1] = RevenantWeaponIDs::SPEAR;

		weapon1AssetPath = "Assets/Sprites/UI/WeaponUI/revenantSword76x303.png";
		weapon2AssetPath = "Assets/Sprites/UI/WeaponUI/revenantSpear99x303.png";
		width1 = 76;
		height1 = 303;
		width2 = 99;
		height2 = 303;

		pos1 = { 100, 20 };
		pos2 = { 500, 20 };
	}
	if (playerClass == PlayerClass::SAGE)
	{
		// Set up variables
		sageIDs[0] = SageWeaponIDs::STAFF;
		sageIDs[1] = SageWeaponIDs::BOOK;

		weapon1AssetPath = "Assets/Sprites/UI/WeaponUI/libro282x216.png";
		weapon2AssetPath = "Assets/Sprites/UI/WeaponUI/libro2_282x216.png";

		width1 = 242;
		height1 = 185;
		width2 = 282;
		height2 = 216;

		pos1 = { 30, 50 };
		pos2 = { 350, 50 };
	}

	weapon1Btn = new GUIButton(pos1, width1, height1, app->textures->Load(weapon1AssetPath));
	weapon2Btn = new GUIButton(pos2, width2, height2, app->textures->Load(weapon2AssetPath));

	buttons.add(weapon1Btn);
	buttons.add(weapon2Btn);

	//text = new Text({ 270, 10 }, "Choose a weapon.", "defaultFont", false);
	Torch1.InitAsTexture(app->textures->Load("Assets/Sprites/UI/antorchas.png"), { 230,205 }, { 0,0,0,0 }, 1, 5, 2, 0, SDL_FLIP_NONE, 0);
	Torch2.InitAsTexture(app->textures->Load("Assets/Sprites/UI/antorchas.png"), { 380,205 }, { 0,0,0,0 }, 1, 5, 2, 0, SDL_FLIP_NONE, 0);

	fondoArmas.InitAsTexture(app->textures->Load("Assets/Sprites/UI/WeaponUI/fondoArmas.png"), posfondo, { 0,0,0,0 }, 0.95, 4, 0, 0, SDL_FLIP_NONE, 0);

	for (int i = 0; i < 8; i++)
	{
		Torch1Anim.PushBack({ 48 * i,2,44,146 });
		Torch2Anim.PushBack({ 48 * i,2,44,146 });
	}

	Torch1Anim.loop = true;
	Torch1Anim.duration = 0.15;
	Torch1Anim.hasIdle = false;

	Torch2Anim.loop = true;
	Torch2Anim.duration = 0.15;
	Torch2Anim.hasIdle = false;
}



	



void WeaponChoosingHUD::Start()
{
	// Triggers an event that stops player from moving.

	app->events->TriggerEvent(GameEvent::STOP_PLAYER_MOVEMENT);

	MenuManager::GetInstance()->canPause = false;

	displayHUD = true;
}

void WeaponChoosingHUD::PreUpdate()
{
	if (!displayHUD) return;

	for (int i = 0; i < buttons.count(); i++)
	{
		buttons[i]->Update();

		if (!buttons[i]->doAction) continue;

		if (playerClass == PlayerClass::REVENANT) playerCombat->ChangeRevenantWeapon(revenantIDs[i]);
		if (playerClass == PlayerClass::SAGE) playerCombat->ChangeSageWeapon(sageIDs[i]);

		buttons[i]->doAction = false;
		
		displayHUD = false;

		app->events->TriggerEvent(GameEvent::RESUME_PLAYER_MOVEMENT);
		MenuManager::GetInstance()->canPause = true;
	}

	if (app->input->usingGameController) GamepadControl();
}

void WeaponChoosingHUD::PostUpdate()
{
	if (!displayHUD) return;

	for (int i = 0; i < buttons.count(); i++)
	{
		buttons[i]->PostUpdate();
	}
	
	app->renderer->AddRenderObjectRenderQueue(fondoArmas);

	Torch1Anim.Update();
	Torch1.section = Torch1Anim.GetCurrentFrame();
	app->renderer->AddRenderObjectRenderQueue(Torch1); 

	Torch2Anim.Update();
	Torch2.section = Torch2Anim.GetCurrentFrame();
	app->renderer->AddRenderObjectRenderQueue(Torch2);

	//text->PostUpdate();
}

void WeaponChoosingHUD::CleanUp()
{
	buttons.clearPtr();
}

void WeaponChoosingHUD::GamepadControl()
{
	// GamePad input control
	if (app->input->GetControllerButton(JoystickButtons::BUTTON_RIGHT) == KEY_DOWN)
		currentOption++;
	else if (app->input->GetControllerButton(JoystickButtons::BUTTON_LEFT) == KEY_DOWN)
	{
		currentOption--;
	}

	int count = buttons.count() - 1;

	if (currentOption > count)
	{
		currentOption = 0;
	}
	else if (currentOption < 0) currentOption = buttons.count() - 1;

	buttons[currentOption]->HoverButton();

	if (app->input->GetControllerButton(JoystickButtons::BUTTON_A) == KEY_DOWN)
	{
		buttons[currentOption]->PressButton(true);
	}
}


