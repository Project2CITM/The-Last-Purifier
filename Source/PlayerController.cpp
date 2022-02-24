#include "PlayerController.h"

PlayerController::PlayerController(std::string name, std::string tag, Application* _app) : GameObject(name, tag, _app)
{

}

void PlayerController::Start() 
{
	// Charge Player Sprites and animations---------------------------------------------

	// Charge texture
	renderObjects[0].InitAsTexture(_app->textures->Load("Assets/Sprites/Player/Knight/KnightAnims.png"), { 0,0,0,0 }, { 0,0,0,0 }, 1, 1, 
		SDL_RendererFlip::SDL_FLIP_NONE, 0, 1.0f, 1.0f);

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
	
	for (int i = 0; i < PLAYER_ANIMATIONS_NUM; i++)
	{
		animations[i].speed = 0.2f;
		animations[i].hasIdle = false;
	}

	currentAnim = PlayerAnim::IDLE;

	// Initialize Animations Manager
	animManager.AddAnimationObject(&animations[(int)PlayerAnim::IDLE]);
	animManager.AddAnimationObject(&animations[(int)PlayerAnim::DASH], false, 1);

	// Initialize physBody
	this->pBody = _app->physics->CreateRectangle({ 100,100 }, 10, 16, this);

}

void PlayerController::PreUpdate()
{
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
	_app->renderer->AddRenderObjectRenderQueue(renderObjects[0]);
}

void PlayerController::CleanUp()
{
	animManager.CleanUp();
}

void PlayerController::MovementUpdate()
{
	animManager.DoAnimation((uint)PlayerAnim::IDLE);

	if (_app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		animManager.DoAnimation((uint)PlayerAnim::DASH);
	}
	// Horizontal
	if (_app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ speed,pBody->body->GetLinearVelocity().y });
	}
	if (_app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ 0,pBody->body->GetLinearVelocity().y });
	}
	if (_app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ -speed,pBody->body->GetLinearVelocity().y });
	}
	if (_app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ 0,pBody->body->GetLinearVelocity().y });
	}

	// Vertical 
	if (_app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, -speed });
	}
	if (_app->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, 0 });
	}
	if (_app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, speed });
	}
	if (_app->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, 0 });
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
