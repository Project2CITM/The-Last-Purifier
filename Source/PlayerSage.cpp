#include "PlayerSage.h"
#include "PlayerStats.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "Animation.h"
#include "PlayerController.h"

PlayerSage::PlayerSage() : Player()
{
	playerClass = PlayerClass::SAGE;
	stats = new PlayerStats(this);
	// In the future here will be initialized every animation and spirte of this class.
	// Charge texture
	controller->renderObjects[0].InitAsTexture(Application::GetInstance()->textures->Load("Assets/Sprites/Player/Sage/sageAnimations80x58.png"), { 0,0 }, { 0,0,0,0 }, 0.7f, 1, 1,
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
		controller->animations[i].speed = 0.1f;
		controller->animations[i].hasIdle = false;
	}

	controller->animations[(int)PlayerAnim::DASH].speed = 0.3f;
}