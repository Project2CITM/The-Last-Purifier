#include "PlayerController.h"

PlayerController::PlayerController(std::string name, std::string tag, Application* _app) : GameObject(name, tag, _app)
{

}

void PlayerController::Start() 
{
	// Charge Player Sprites and animations---------------------------------------------

	// Charge texture
	renderObjects[0].InitAsTexture(_app->textures->Load("Assets/Sprites/Player/Knight/KnightAnims.png"), { 0,0 }, { 0,0,0,0 }, 1.0f, 1, 1,
		0, SDL_RendererFlip::SDL_FLIP_NONE, 1.0f);

	renderObjects[0].textureCenterX = 32;
	renderObjects[0].textureCenterY = 15;

	// Create animations

	for (int i = 0; i < 15; i++)
	{
		// Idle anim initialize
		animations[(int)PlayerAnim::IDLE].PushBack({ 64 * i, 0, 64, 30 });
		animations[(int)PlayerAnim::IDLE].loop = true;

		// Roll anim initialize
		animations[(int)PlayerAnim::DASH].PushBack({ 64 * i, 30, 64, 30 });
		animations[(int)PlayerAnim::DASH].loop = false;
	}
	
	for (int i = 0; i < 8; i++)
	{
		animations[(int)PlayerAnim::RUN].PushBack({ 64 * i, 60, 64, 30 });
		animations[(int)PlayerAnim::RUN].loop = true;
	}

	for (int i = 0; i < PLAYER_ANIMATIONS_NUM; i++)
	{
		animations[i].speed = 0.2f;
		animations[i].hasIdle = false;
	}

	currentAnim = PlayerAnim::IDLE;

	// Initialize Animations Manager 
	// WARNING: THEY MUST BE ADDED FOLLOWING THE ORDER OF THE PlayerAnim ENUM
	animManager.AddAnimationObject(&animations[(int)PlayerAnim::IDLE]);
	animManager.AddAnimationObject(&animations[(int)PlayerAnim::DASH], false, 1);
	animManager.AddAnimationObject(&animations[(int)PlayerAnim::RUN]);

	// Initialize physBody
	this->pBody = _app->physics->CreateRectangle({ 100,100 }, 10, 16, this);

}

void PlayerController::PreUpdate()
{
	// Check dash cooldown
	if (isDashing)
	{
		dashCounter--;
		// If Cooldown is done, you stop dashing
		if (dashCounter <= 0)
		{
			isDashing = false;
			pBody->body->SetLinearVelocity({ 0,0 });
		}
	}

	// Get Movement Input
	MovementUpdate();

	// If our current animation has finished, we reset it manually. This is made for DASH and ATTACK animations.
	// When these animations end, you must Reset them for the next time you'll use them
	if (animations[(int)currentAnim].HasFinished()) animations[(int)currentAnim].Reset();
}

void PlayerController::Update()
{
	currentAnim = (PlayerAnim)animManager.currentAnimation;
	// Update animation
	animations[(int)currentAnim].Update();	
}

void PlayerController::PostUpdate()
{
	renderObjects[0].section = animations[(int)currentAnim].GetCurrentFrame();
	renderObjects[0].destRect.x = GetDrawPosition().x;
	renderObjects[0].destRect.y = GetDrawPosition().y;

	if (lookingDir == LookingDirection::LEFT) renderObjects[0].flip = SDL_FLIP_HORIZONTAL;
	else if (lookingDir == LookingDirection::RIGHT) renderObjects[0].flip = SDL_FLIP_NONE;

	_app->renderer->AddRenderObjectRenderQueue(renderObjects[0]);
}

void PlayerController::CleanUp()
{
	animManager.CleanUp();
}

void PlayerController::MovementUpdate()
{
	// By default, the player is always IDLE
	animManager.DoAnimation((uint)PlayerAnim::IDLE);

	if (_app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (!isDashing)
		{
			//Reset Dash animation in case it hadn't finished yet
			animations[(int)PlayerAnim::DASH].Reset();
			// Play Dash Animation
			animManager.DoAnimation((uint)PlayerAnim::DASH);
			// do the dash
			DashOn();
		}
	}

	// If we are dashing, all other movement is disabled
	if (isDashing) return;

	// Vertical 
	if (_app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, -speed });
		animManager.DoAnimation((uint)PlayerAnim::RUN);
		lookingDir = LookingDirection::UP;
	}
	if (_app->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, 0 });
	}
	if (_app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, speed });
		animManager.DoAnimation((uint)PlayerAnim::RUN);
		lookingDir = LookingDirection::DOWN;
	}
	if (_app->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, 0 });
	}

	// Horizontal
	if (_app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ speed,pBody->body->GetLinearVelocity().y });
		animManager.DoAnimation((uint)PlayerAnim::RUN);
		lookingDir = LookingDirection::RIGHT;
	}
	if (_app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ 0,pBody->body->GetLinearVelocity().y });
	}
	if (_app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ -speed,pBody->body->GetLinearVelocity().y });
		animManager.DoAnimation((uint)PlayerAnim::RUN);
		lookingDir = LookingDirection::LEFT;
	}
	if (_app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ 0,pBody->body->GetLinearVelocity().y });
	}

	// If we are moving dyagonally, we lower our velocity
	if (abs(pBody->body->GetLinearVelocity().x) > 0 && abs(pBody->body->GetLinearVelocity().y) > 0)
	{
		b2Vec2 reducedVelocity = pBody->body->GetLinearVelocity();
		reducedVelocity.x *= 0.7f;
		reducedVelocity.y *= 0.7f;
		pBody->body->SetLinearVelocity(reducedVelocity);
	}
}

void PlayerController::DashOn()
{
	isDashing = true;
	dashCounter = dashTime;

	b2Vec2 dir = { GetPlayerToMouseVector().x * dashDistance, GetPlayerToMouseVector().y * dashDistance};

	pBody->body->SetLinearVelocity(dir);
}

fPoint PlayerController::GetPlayerToMouseVector()
{
	fPoint vec = { (float)(_app->input->GetMouseX() - GetScreenPosition().x), (float)(_app->input->GetMouseY() - GetScreenPosition().y ) };
	vec = vec.Normalize();

	return vec;
}
