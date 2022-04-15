#include "Ghoul.h"
#include "ModulePhysics.h"

Ghoul::Ghoul(iPoint pos) : Enemy("ghoul")
{
	InitRenderObjectWithXml("ghoul");

	InitAnimation();

	pBody = app->physics->CreateCircle(pos.x, pos.y, 12, this);

	this->position = pos;
}

Ghoul::~Ghoul()
{
}

void Ghoul::PreUpdate()
{
	Enemy::PreUpdate();
}

void Ghoul::Update()
{
	Enemy::Update();
}

void Ghoul::PostUpdate()
{
	animations[(int)currentState].Update();

	int frame = animations[(int)currentState].getCurrentFrameI();

	renderObjects[0].section = animations[(int)currentState].GetCurrentFrame();

	Enemy::PostUpdate();
}

void Ghoul::InitAnimation()
{
	// Create animations
	for (int i = 0; i < 4; i++)
	{
		// Idle anim initialize
		animations[(int)GhoulState::IDLE].PushBack({ 32 * i, 0, 32, 32 });
		animations[(int)GhoulState::IDLE].loop = true;
	}

	for (int i = 0; i < 8; i++)
	{
		// Run anim initialize
		animations[(int)GhoulState::RUN].PushBack({ 32 * i, 32, 32, 32 });
		animations[(int)GhoulState::RUN].loop = true;
	}

	for (int i = 0; i < 6; i++)
	{
		// Attack anim initialize
		animations[(int)GhoulState::ATTACK].PushBack({ 32 * i, 64, 32, 32 });
		animations[(int)GhoulState::ATTACK].loop = true;
	}

	for (int i = 0; i < 4; i++)
	{
		// Hit anim initialize
		animations[(int)GhoulState::HIT].PushBack({ 32 * i, 96, 32, 32 });
		animations[(int)GhoulState::HIT].loop = true;
	}

	for (int i = 0; i < 6; i++)
	{
		// Die anim initialize
		animations[(int)GhoulState::DIE].PushBack({ 32 * i, 128, 32, 32 });
		animations[(int)GhoulState::DIE].loop = false;
	}

	for (int i = 0; i < (int)GhoulState::MAX; i++)
	{
		animations[i].hasIdle = false;
		animations[i].speed = 0.2f;
	}

	currentState = GhoulState::IDLE;
}
