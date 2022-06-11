#include "Player.h"
#include "PlayerController.h"
#include "Application.h"
#include "PlayerStats.h"
#include "ModuleTextures.h"
#include "PlayerCombat.h"

Player::Player()
{
	controller = new PlayerController("Player", "Player", this);
}

void Player::CleanUp()
{
	// THE CONTROLLER IS CLEANED UP BY THE SCENE, AS IT IS A GAMEOBJECT AND, THEREFOR, MANAGED BY THE SCENE
	if (stats != nullptr)
	{
		stats->CleanUp();
		RELEASE(stats);
	}
	if (controller != nullptr)controller->pendingToDelete = true;
		
}

void Player::ChangeShield(int num)
{
	shield += num;
	if (shield < 0) shield = 0;
}

void Player::AddSouls(int num)
{
	souls+= num * this->soulGain;
}

void Player::ChangeClass(PlayerClass c)
{
	if (playerClass == c) return;

	playerClass = c;
	InitClassSprites(c);
	controller->combat->SetClassWeaponCD();
	stats->SavePlayerProgress();
}

void Player::InitClassSprites(PlayerClass c)
{
	switch (c)
	{
	case PlayerClass::REVENANT:
		InitRevenantSprites();
		break;
	case PlayerClass::SAGE:
		InitSageSprites();
		break;
	}
}

void Player::InitRevenantSprites()
{
	for (int i = 0; i < PLAYER_ANIMATIONS_NUM; i++)
	{
		controller->animations[i].EmptyAnimation();
	}

	// Charge texture
	controller->renderObjects[0].InitAsTexture(Application::GetInstance()->textures->Load("Assets/Sprites/Player/playerSpritesheet.png"), { 0,0 }, { 0,0,0,0 }, 0.7f, 1, 1,
		0, SDL_RendererFlip::SDL_FLIP_NONE, 1.0f);

	controller->renderObjects[0].textureCenterX = 40;
	controller->renderObjects[0].textureCenterY = 29;

	controller->textureOffset = { 10,-15 };

	// Create animations
	for (int i = 0; i < 8; i++)
	{
		// Run horizontal animation
		controller->animations[(int)PlayerAnim::RUN_DOWN].PushBack({ 70 * i, 0, 70, 70 });
		controller->animations[(int)PlayerAnim::RUN_DOWN].loop = true;

	}

	for (int i = 0; i < 4; i++)
	{
		// Run vertical animation
		controller->animations[(int)PlayerAnim::RUN_HORIZONTAL].PushBack({ 70 * i, 70, 70, 70 });
		controller->animations[(int)PlayerAnim::RUN_HORIZONTAL].loop = false;

		controller->animations[(int)PlayerAnim::DASH_HORIZONTAL].PushBack({ 70 * i, 280, 70, 70 });
		controller->animations[(int)PlayerAnim::DASH_HORIZONTAL].loop = false;

		controller->animations[(int)PlayerAnim::RUN_UP].PushBack({ 70 * i, 140, 70, 70 });
		controller->animations[(int)PlayerAnim::RUN_UP].loop = false;

		controller->animations[(int)PlayerAnim::DASH_UP].PushBack({ 70 * i, 350, 70, 70 });
		controller->animations[(int)PlayerAnim::DASH_UP].loop = false;

		controller->animations[(int)PlayerAnim::DASH_DOWN].PushBack({ 70 * i, 210, 70, 70 });
		controller->animations[(int)PlayerAnim::DASH_DOWN].loop = true;
	}

	for (int i = 0; i < PLAYER_ANIMATIONS_NUM; i++)
	{
		controller->animations[i].duration = 0.16f;
		controller->animations[i].hasIdle = false;
	}

	this->classColor = { 255,100,0,255 };
}

void Player::InitSageSprites()
{
	for (int i = 0; i < PLAYER_ANIMATIONS_NUM; i++)
	{
		controller->animations[i].EmptyAnimation();
	}

	// Charge texture
	controller->renderObjects[0].InitAsTexture(Application::GetInstance()->textures->Load("Assets/Sprites/Player/playerSpritesheet.png"), { 0,0 }, { 0,0,0,0 }, 0.7f, 1, 1,
		0, SDL_RendererFlip::SDL_FLIP_NONE, 1.0f);

	controller->renderObjects[0].textureCenterX = 40;
	controller->renderObjects[0].textureCenterY = 29;

	controller->textureOffset = { 10,-15 };

	// Create animations
	for (int i = 0; i < 8; i++)
	{
		// Run horizontal animation
		controller->animations[(int)PlayerAnim::RUN_DOWN].PushBack({ 70 * i, 0, 70, 70 });
		controller->animations[(int)PlayerAnim::RUN_DOWN].loop = true;

		// Run horizontal animation
		controller->animations[(int)PlayerAnim::DASH_DOWN].PushBack({ 70 * i, 0, 70, 70 });
		controller->animations[(int)PlayerAnim::DASH_DOWN].loop = true;
	}

	for (int i = 0; i < 4; i++)
	{
		// Run vertical animation
		controller->animations[(int)PlayerAnim::RUN_HORIZONTAL].PushBack({ 70 * i, 70, 70, 70 });
		controller->animations[(int)PlayerAnim::RUN_HORIZONTAL].loop = false;

		// Run vertical animation
		controller->animations[(int)PlayerAnim::DASH_HORIZONTAL].PushBack({ 70 * i, 70, 70, 70 });
		controller->animations[(int)PlayerAnim::DASH_HORIZONTAL].loop = false;

		controller->animations[(int)PlayerAnim::RUN_UP].PushBack({ 70 * i, 140, 70, 70 });
		controller->animations[(int)PlayerAnim::RUN_UP].loop = false;

		controller->animations[(int)PlayerAnim::DASH_UP].PushBack({ 70 * i, 140, 70, 70 });
		controller->animations[(int)PlayerAnim::DASH_UP].loop = false;
	}
	
	for (int i = 0; i < PLAYER_ANIMATIONS_NUM; i++)
	{
		controller->animations[i].duration = 0.16f;
		controller->animations[i].hasIdle = false;
	}

	this->classColor = { 255,255,255,255 };
}