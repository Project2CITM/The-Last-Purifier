#include "PlayerRevenant.h"
#include "Application.h"
#include "PlayerController.h"
#include "PlayerStats.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "Animation.h"

PlayerRevenant::PlayerRevenant() : Player()
{
	playerClass = PlayerClass::REVENANT;
	stats = new PlayerStats(this);
	// In the future here will be initialized every animation and spirte of this class.

	// Charge texture
	controller->renderObjects[0].InitAsTexture(Application::GetInstance()->textures->Load("Assets/Sprites/Player/Knight/KnightAnims.png"), { 0,0 }, { 0,0,0,0 }, 1.2f, 1, 1,
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