#include "PlayerController.h"
#include "Player.h"

PlayerController::PlayerController(std::string name, std::string tag, Application* _app, Player* player) : GameObject(name, tag, _app)
{
	this->player = player;
	combat = new PlayerCombat(this->player);
}

void PlayerController::Start() 
{
	#pragma region TEMPORARY_CODE
	// Every Animation and spirtes charging should be in the PlayerRevenant and PlayerSage classes!!!!!
	// TEMPORARY CODE!---------------------------------------------------
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
	#pragma endregion
	
	currentAnim = PlayerAnim::IDLE;

	// Initialize States 
	// WARNING: They must be added following the order specified on the PlayerState Enum!!!
	stateMachine.AddState("idle", 0);			//IDLE = 0
	stateMachine.AddState("run", 0);			//RUN = 1
	stateMachine.AddState("attack", 1, 32);		//ATTACK = 2
	stateMachine.AddState("dash", 2, 64);		//DASH = 3

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

	// Get Combat Input
	CombatUpdate();

	// If our current animation has finished, we reset it manually. This is made for DASH and ATTACK animations.
	// When these animations end, you must Reset them for the next time you'll use them
	if (animations[(int)currentAnim].HasFinished()) animations[(int)currentAnim].Reset();
}

void PlayerController::Update()
{
	// Update State Machine
	stateMachine.Update();

	// Update current player State
	currentState = (PlayerState)stateMachine.GetCurrentState();

	//printf("CurrentState %d\n", stateMachine.GetCurrentState());

	// Update animation
	animations[(int)currentAnim].Update();	
}

void PlayerController::PostUpdate()
{
	// Update current Animation state 
	// For now it is the same as Player State, if this changes overtime, there has to be a switch here to translate between current player State
	// and current Animation State
	currentAnim = (PlayerAnim)currentState;

	renderObjects[0].section = animations[(int)currentAnim].GetCurrentFrame();
	renderObjects[0].destRect.x = GetDrawPosition().x;
	renderObjects[0].destRect.y = GetDrawPosition().y;

	if (lookingDir == LookingDirection::LEFT) renderObjects[0].flip = SDL_FLIP_HORIZONTAL;
	else if (lookingDir == LookingDirection::RIGHT) renderObjects[0].flip = SDL_FLIP_NONE;

	_app->renderer->AddRenderObjectRenderQueue(renderObjects[0]);
}

void PlayerController::CleanUp()
{
	if (combat != nullptr)
	{
		combat->CleanUp();
		RELEASE(combat);
	}
}

void PlayerController::MovementUpdate()
{
	// By default, the player is always IDLE
	stateMachine.ChangeState((uint)PlayerState::IDLE);

	if (_app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (!isDashing)
		{
			//Reset Dash animation in case it hadn't finished yet
			animations[(int)PlayerAnim::DASH].Reset();
		
			//Change Player State
			stateMachine.ChangeState((uint)PlayerState::DASH);

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

		//Change Player State
		stateMachine.ChangeState((uint)PlayerState::RUN);

		lookingDir = LookingDirection::UP;
	}
	if (_app->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, 0 });
	}
	if (_app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, speed });

		//Change Player State
		stateMachine.ChangeState((uint)PlayerState::RUN);

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

		//Change Player State
		stateMachine.ChangeState((uint)PlayerState::RUN);

		lookingDir = LookingDirection::RIGHT;
	}
	if (_app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ 0,pBody->body->GetLinearVelocity().y });
	}
	if (_app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ -speed,pBody->body->GetLinearVelocity().y });

		//Change Player State
		stateMachine.ChangeState((uint)PlayerState::RUN);

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
		reducedVelocity.x *= 0.8f;
		reducedVelocity.y *= 0.8f;
		pBody->body->SetLinearVelocity(reducedVelocity);
	}
}

void PlayerController::CombatUpdate()
{
	// Check for attack and Spell input
	if (_app->input->GetMouseButton(1) == KEY_DOWN)
	{
		combat->Attack();
	}
	else if (_app->input->GetMouseButton(3) == KEY_DOWN)
	{
		combat->CastSpell();
	}

	// Check for spell changing input

	if (_app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		combat->ChangeSelectedSpellSlot(-1);
	}
	else if (_app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		combat->ChangeSelectedSpellSlot(1);
	}
}

void PlayerController::DashOn()
{
	isDashing = true;
	dashCounter = dashTime;

	b2Vec2 dir = pBody->body->GetLinearVelocity();
	dir.Normalize();
	dir.x *= dashDistance;
	dir.y *= dashDistance;

	// if we have no velocity, it means we are idle.
	if (dir.x == 0 && dir.y == 0)
	{
		// So we dash on the current direction we are facing
		switch (lookingDir)
		{
		case LookingDirection::UP:
			dir = {0, (float)-1 * dashDistance};
			break;
		case LookingDirection::DOWN:
			dir = {0, (float)1 * dashDistance };
			break;
		case LookingDirection::LEFT:
			dir = { (float)-1 * dashDistance, 0};
			break;
		case LookingDirection::RIGHT:
			dir = { (float)1 * dashDistance, 0};
			break;
		}

	}

	pBody->body->SetLinearVelocity(dir);
}

fPoint PlayerController::GetPlayerToMouseVector()
{
	fPoint vec = { (float)(_app->input->GetMouseX() - GetScreenPosition().x), (float)(_app->input->GetMouseY() - GetScreenPosition().y ) };
	vec = vec.Normalize();

	return vec;
}
