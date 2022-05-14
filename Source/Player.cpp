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
	for (int i = 0; i < 4; i++)
	{
		controller->animations[i].EmptyAnimation();
	}

	// Charge texture
	controller->renderObjects[0].InitAsTexture(Application::GetInstance()->textures->Load("Sprites/Player/Knight/KnightAnims.png"), { 0,0 }, { 0,0,0,0 }, 1.2f, 1, 1,
		0, SDL_RendererFlip::SDL_FLIP_NONE, 1.0f);

	controller->renderObjects[0].textureCenterX = 32;
	controller->renderObjects[0].textureCenterY = 15;

	controller->textureOffset = { 0,-13 };

	// Create animations
	for (int i = 0; i < 15; i++)
	{
		// Idle anim initialize
		controller->animations[(int)PlayerAnim::IDLE].PushBack({ 64 * i, 0, 64, 30 });
		controller->animations[(int)PlayerAnim::IDLE].loop = true;

		// Roll anim initialize
		controller->animations[(int)PlayerAnim::DASH].PushBack({ 64 * i, 30, 64, 30 });
		controller->animations[(int)PlayerAnim::DASH].loop = false;
	}

	for (int i = 0; i < 8; i++)
	{
		controller->animations[(int)PlayerAnim::RUN].PushBack({ 64 * i, 60, 64, 30 });
		controller->animations[(int)PlayerAnim::RUN].loop = true;
	}

	for (int i = 0; i < PLAYER_ANIMATIONS_NUM; i++)
	{
		controller->animations[i].duration = 0.081f;
		controller->animations[i].hasIdle = false;
	}

	controller->animations[(int)PlayerAnim::DASH].duration = 0.04f;
}

void Player::InitSageSprites()
{
	for (int i = 0; i < 4; i++)
	{
		controller->animations[i].EmptyAnimation();
	}

	// Charge texture
	controller->renderObjects[0].InitAsTexture(Application::GetInstance()->textures->Load("Sprites/Player/Sage/sageAnimations80x58.png"), { 0,0 }, { 0,0,0,0 }, 0.7f, 1, 1,
		0, SDL_RendererFlip::SDL_FLIP_NONE, 1.0f);

	controller->renderObjects[0].textureCenterX = 40;
	controller->renderObjects[0].textureCenterY = 29;

	controller->textureOffset = { 20,0 };

	// Create animations
	for (int i = 0; i < 6; i++)
	{
		// Roll anim initialize
		controller->animations[(int)PlayerAnim::DASH].PushBack({ 80 * i, 58, 80, 58 });
		controller->animations[(int)PlayerAnim::DASH].loop = false;
	}

	for (int i = 0; i < 10; i++)
	{
		controller->animations[(int)PlayerAnim::RUN].PushBack({ 80 * i, 0, 80, 58 });
		controller->animations[(int)PlayerAnim::RUN].loop = true;

		// Idle anim initialize
		controller->animations[(int)PlayerAnim::IDLE].PushBack({ 80 * i, 0, 80, 58 });
		controller->animations[(int)PlayerAnim::IDLE].loop = true;
	}

	for (int i = 0; i < PLAYER_ANIMATIONS_NUM; i++)
	{
		controller->animations[i].duration = 0.16f;
		controller->animations[i].hasIdle = false;
	}

	controller->animations[(int)PlayerAnim::DASH].duration = 0.0640f;
}