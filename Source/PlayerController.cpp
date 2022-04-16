#include "PlayerController.h"
#include "Player.h"
#include "PlayerCombat.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"

#include "RoomManager.h"
#include "Room.h"
#include "PlayerStats.h"

#include "ModuleMap.h"

PlayerController::PlayerController(std::string name, std::string tag, Player* player) : GameObject(name, tag)
{
	this->player = player;
}

void PlayerController::Start() 
{
	combat = new PlayerCombat("playerAttack", "AttackArea", this->player);

	player->stats->Start();

	#pragma region TEMPORARY_CODE
	// Every Animation and spirtes charging should be in the PlayerRevenant and PlayerSage classes!!!!!
	// TEMPORARY CODE!---------------------------------------------------
	// Charge Player Sprites and animations---------------------------------------------

	// Charge texture
	renderObjects[0].InitAsTexture(app->textures->Load("Assets/Sprites/Player/Knight/KnightAnims.png"), { 0,0 }, { 0,0,0,0 }, 1.0f, 1, 1,
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

	#pragma endregion

	for (int i = 0; i < PLAYER_ANIMATIONS_NUM; i++)
	{
		animations[i].speed = 0.2f;
		animations[i].hasIdle = false;
	}

	animations[(int)PlayerAnim::DASH].speed = 0.4f;
	
	// Initialize movement variables
	speed = player->movementSpeed;

	currentAnim = PlayerAnim::IDLE;

	// Initialize States 
	// WARNING: They must be added following the order specified on the PlayerState Enum!!!
	stateMachine.AddState("idle", 0);			//IDLE = 0
	stateMachine.AddState("run", 0);			//RUN = 1
	stateMachine.AddState("attack", 1, 5);		//ATTACK = 2
	stateMachine.AddState("dash", 2, 25);		//DASH = 3

	// Initialize physic body
	CreatePhysBody();
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

	// Check invulnerability counter
	if (invulnerabilityCounter > 0) --invulnerabilityCounter;
	if (invulnerabilityCounter <= 0) isInvulnerable = false;

	// Every frame set the linear velocity to 0 in case we are not moving and we are not dashing
	// This is done to prevent drifting when applying forces from other bodies into the player body.
	if (!isDashing)pBody->body->SetLinearVelocity(b2Vec2(0, 0));

	// Get Movement Input
	if (app->input->usingGameController) MovementUpdateController();
	else MovementUpdateKeyboard();

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
	renderObjects[0].destRect.y = GetDrawPosition().y-13;

	if (lookingDir == LookingDirection::LEFT) renderObjects[0].flip = SDL_FLIP_HORIZONTAL;
	else if (lookingDir == LookingDirection::RIGHT) renderObjects[0].flip = SDL_FLIP_NONE;

	renderObjects[0].scale = 1.2f;
	app->renderer->AddRenderObjectRenderQueue(renderObjects[0]);
}

void PlayerController::CleanUp()
{
	// PlayerCombat is not deleted here because it gets added automatically to the scene.
	// Therefor, is deleted by the scene.
	if (combat != nullptr)
	{
		combat->pendingToDelete = true;
	}

	GameObject::CleanUp();
}

void PlayerController::CreatePhysBody()
{
	int playerChain[8]
	{
		0,0,
		16,0,
		16,8,
		0,8
	};

	this->pBody = app->physics->CreateChainObj(((MAX_ROOMS_COLUMNS + 1) * MAX_ROOM_TILES_COLUMNS * TILE_SIZE) / 2,
		((MAX_ROOMS_ROWS + 1) * MAX_ROOM_TILES_ROWS * TILE_SIZE) / 2,
		playerChain, 8, true, this);
	this->pBody->body->SetFixedRotation(true);

	// Left Circle
	b2FixtureDef circleL;
	b2CircleShape shape;
	shape.m_radius = PIXELS_TO_METER(6);
	shape.m_p = b2Vec2(0, PIXELS_TO_METER(4));
	circleL.shape = &shape;

	pBody->body->CreateFixture(&circleL);

	// Right Circle
	b2FixtureDef circleR;
	b2CircleShape shape2;
	shape2.m_radius = PIXELS_TO_METER(6);
	shape2.m_p = b2Vec2(PIXELS_TO_METER(16), PIXELS_TO_METER(4));
	circleR.shape = &shape2;

	pBody->body->CreateFixture(&circleR);

	b2Filter filter;
	filter.categoryBits = app->physics->PLAYER_LAYER;
	filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->PLAYER_LAYER;

	b2Fixture* bodyFixture = pBody->body->GetFixtureList();
	while (bodyFixture != nullptr)
	{
		bodyFixture->SetFilterData(filter);
		bodyFixture = bodyFixture->GetNext();
	}

	// Initialize enemy trigger body
	enemyTrigger = new Trigger(GetPosition(), 8, 16, this, "Player");
	enemyTrigger->positionOffset = { 8, -12 };
	enemyTrigger->pBody->body->GetFixtureList()[0].SetFilterData(filter);
}

void PlayerController::MovementUpdateKeyboard()
{
	// By default, the player is always IDLE
	stateMachine.ChangeState((uint)PlayerState::IDLE);

	// If we are dashing, all other movement is disabled
	if (isDashing) return;

	// Vertical 
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, -speed });

		//Change Player State
		stateMachine.ChangeState((uint)PlayerState::RUN);

		lookingDir = LookingDirection::UP;
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, 0 });
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, speed });

		//Change Player State
		stateMachine.ChangeState((uint)PlayerState::RUN);

		lookingDir = LookingDirection::DOWN;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, 0 });
	}

	// Horizontal
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ speed,pBody->body->GetLinearVelocity().y });

		//Change Player State
		stateMachine.ChangeState((uint)PlayerState::RUN);

		lookingDir = LookingDirection::RIGHT;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ 0,pBody->body->GetLinearVelocity().y });
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ -speed,pBody->body->GetLinearVelocity().y });

		//Change Player State
		stateMachine.ChangeState((uint)PlayerState::RUN);

		lookingDir = LookingDirection::LEFT;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ 0,pBody->body->GetLinearVelocity().y });
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
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

	// If we are moving dyagonally, we lower our velocity
	if (abs(pBody->body->GetLinearVelocity().x) > 0 && abs(pBody->body->GetLinearVelocity().y) > 0)
	{
		b2Vec2 reducedVelocity = pBody->body->GetLinearVelocity();
		reducedVelocity.x *= 0.8f;
		reducedVelocity.y *= 0.8f;
		pBody->body->SetLinearVelocity(reducedVelocity);
	}
}

void PlayerController::MovementUpdateController()
{
	// By default, the player is always IDLE
	stateMachine.ChangeState((uint)PlayerState::IDLE);

	if (app->input->GetControllerButton(BUTTON_B) == KEY_DOWN)
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

	int leftX;
	int leftY;
	// Get controller axis
	leftX = app->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTX);
	leftY = app->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTY);

	// If number is too low, ignore input
	if (abs(leftX) < 10000) leftX = 0;
	if (abs(leftY) < 10000) leftY = 0;

	// Get only direction
	b2Vec2 direction = { (float)leftX, (float)leftY };
	direction.Normalize();
	
	// Determine looking direction from direction b2vec2 vector at previous step.
	int xInput = direction.x;
	int yInput = direction.y;

	if (abs(xInput) > abs(yInput))
	{
		if (direction.x > 0)
		{
			stateMachine.ChangeState((uint)PlayerState::RUN);

			lookingDir = LookingDirection::RIGHT;
		}
		else if (direction.x < 0)
		{
			stateMachine.ChangeState((uint)PlayerState::RUN);

			lookingDir = LookingDirection::LEFT;
		}
	}
	else
	{
		if (direction.y < 0)
		{
			stateMachine.ChangeState((uint)PlayerState::RUN);

			lookingDir = LookingDirection::UP;
		}
		else if (direction.y > 0)
		{
			//Change Player State
			stateMachine.ChangeState((uint)PlayerState::RUN);

			lookingDir = LookingDirection::DOWN;
		}
	}

	// Add speed to direction vector
	direction.x *= speed;
	direction.y *= speed;
	// Apply speed
	pBody->body->SetLinearVelocity(direction);
}

void PlayerController::CombatUpdate()
{
	// Check for attack and Spell input
	if (app->input->GetMouseButton(1) == KEY_DOWN || app->input->GetControllerButton(BUTTON_X) == KEY_DOWN)
	{
		combat->Attack();
	}
	else if (app->input->GetMouseButton(3) == KEY_DOWN || app->input->GetControllerButton(BUTTON_A) == KEY_DOWN)
	{
		combat->CastSpell();
	}

	// Check for spell changing input

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN || app->input->GetControllerButton(BUTTON_LEFT_SHOULDER) == KEY_DOWN)
	{
		combat->ChangeSelectedSpellSlot(-1);
	}
	else if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetControllerButton(BUTTON_RIGHT_SHOULDER) == KEY_DOWN)
	{
		combat->ChangeSelectedSpellSlot(1);
	}
}

void PlayerController::DashOn()
{
	Invulnerability(dashInvulnerability);

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
	fPoint vec = { (float)(app->input->GetMouseX() - GetScreenPosition().x), (float)(app->input->GetMouseY() - GetScreenPosition().y ) };
	vec = vec.Normalize();

	return vec;
}

void PlayerController::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject != nullptr)
	{
		if (col->gameObject->CompareTag("HubDoorIN"))
		{
			app->map->roof = false;
		}

		if (col->gameObject->CompareTag("HubDoorOUT"))
		{
			app->map->roof = true;
		}
	}
}

void PlayerController::OnCollisionExit(PhysBody* col)
{

}

void PlayerController::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->name == "DamageArea")
	{
		if (isInvulnerable) return;
		DamageArea* dArea = (DamageArea*)col->gameObject;
		if (dArea->damage != nullptr)Hit(*dArea->damage);
		if (dArea->stunTime != nullptr)Stun(*dArea->stunTime);
	}

	if (col->gameObject->CompareTag("Enemy"))
	{
		Trigger* enemyTrigger = (Trigger*)col->gameObject;
	
		Enemy* enemy = (Enemy*)enemyTrigger->GetParent();

		if (enemy == nullptr) return;

		Hit(enemy->GetDamage());
	}
}

void PlayerController::Hit(int damage)
{
	player->hpPlayer -= damage;
	if (player->hpPlayer <= 0) printf("Player Die!!\n");
}

void PlayerController::Stun(int frames)
{
}

void PlayerController::Invulnerability(int frames)
{
	isInvulnerable = true;
	invulnerabilityCounter = frames;
}
