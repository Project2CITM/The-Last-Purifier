#include "BossTransition.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

BossTransition::BossTransition()
{
	app = Application::GetInstance();

	SDL_Texture* tex = app->textures->Load("Assets/Sprites/Enemies/Golem/Boss_transition.png");

	barTop.InitAsTexture(tex, { -1000,0}, { 0, 0, 320, 52 }, 2, 4, 100, 0, SDL_FLIP_NONE, 0);
	barTop.screenDetect = false;

	barBottom.InitAsTexture(tex, { 1000, 256}, { 0, 128, 320, 52 }, 2, 4, 100, 0, SDL_FLIP_NONE, 0);
	barBottom.screenDetect = false;

	boss.InitAsTexture(tex, { 800, 0}, { 320, 0, 180, 180 }, 2, 4, 101, 0, SDL_FLIP_NONE, 0);
	boss.screenDetect = false;
}

BossTransition::~BossTransition()
{
}

void BossTransition::Update()
{
	if (!transitionStart || transitionEnd) return;

	if (barTop.destRect.x < 0)
	{
		barTop.destRect.x += barTopMoveSpeed;

		if (barTop.destRect.x > 0) barTop.destRect.x = 0;

		barTopMoveSpeed = (abs(barTop.destRect.x) / 10) + 5;
	}
	
	if (barBottom.destRect.x > 0)
	{
		barBottom.destRect.x -= barBottomMoveSpeed;

		if (barBottom.destRect.x < 0) barBottom.destRect.x = 0;

		barBottomMoveSpeed = (abs(barBottom.destRect.x) / 10) + 5;
	}
	else if (boss.destRect.x > 300)
	{
		boss.destRect.x -= bossMoveSpeed;

		if (boss.destRect.x <= 300) boss.destRect.x = 300;

		bossMoveSpeed = (abs(barBottom.destRect.x) / 5) + 5;
	}
	else if(finishCoolDown-- <= 0) transitionEnd = true;
}

void BossTransition::PostUpdate()
{
	if (!transitionStart || transitionEnd) return;

	app->renderer->AddRenderObjectRenderQueue(barTop);

	app->renderer->AddRenderObjectRenderQueue(barBottom);

	app->renderer->AddRenderObjectRenderQueue(boss);
}
