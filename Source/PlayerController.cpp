#include "PlayerController.h"
#include "Player.h"
#include "PlayerCombat.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleAudio.h"

#include "RoomManager.h"
#include "Room.h"
#include "PlayerStats.h"
#include "DamageArea.h"

#include "Enemy.h"

#include "ModuleMap.h"
#include "PlayerShadow.h"
#include "ParticleTeleport.h"

PlayerController::PlayerController(std::string name, std::string tag, Player* player) : GameObject(name, tag)
{
	this->listenTo[0] = GameEvent::COMPLETE_ROOM;
	this->listenTo[1] = GameEvent::PLAYER_DIE;
	this->listenTo[2] = GameEvent::RESUME_PLAYER_MOVEMENT;
	this->listenTo[3] = GameEvent::STOP_PLAYER_MOVEMENT;
	this->listenTo[4] = GameEvent::ENEMY_HIT;

	app->events->AddListener(this);
	this->player = player;

	playerdodgeFX = app->audio->LoadFx("Audio/SFX/Player/sfx_playerDodge.wav");
	playerhitFX = app->audio->LoadFx("Audio/SFX/Player/sfx_playerHit2");
	this->playerShadow = new PlayerShadow(this);
}

void PlayerController::Start() 
{
	// Initialize States 
	// WARNING: They must be added following the order specified on the PlayerState Enum!!!
	stateMachine.AddState("idle", 0);			//IDLE = 0
	stateMachine.AddState("run", 0);			//RUN = 1
	stateMachine.AddState("attack", 1, 80);		//ATTACK = 2
	stateMachine.AddState("dash", 2, 400);		//DASH = 3

	combat = new PlayerCombat("playerAttack", "AttackArea", this->player);

	player->stats->Start();

	// Initialize movement variables
	speed = player->movementSpeed;

	currentAnim = PlayerAnim::IDLE;

	// Initialize physic body
	CreatePhysBody();
}

void PlayerController::PreUpdate()
{
	if (pendingToDelete) return;

	playerTimer.Update();

	// Check invulnerability counter
	if (invulnerabilityCounter > 0)
	{
		if (enemyTrigger) enemyTrigger->pBody->body->SetActive(false);

		invulnerabilityCounter -= playerTimer.getDeltaTime() * 1000;
	}
	if (invulnerabilityCounter <= 0)
	{
		if(enemyTrigger) enemyTrigger->pBody->body->SetActive(true);

		isInvulnerable = false;
	}

	// Check dash cooldown
	if (isDashing)
	{
		// Counter------------------------
		dashCounter -= playerTimer.getDeltaTime() * 1000;
		// If Cooldown is done, you stop dashing
		if (dashCounter <= 0)
		{
			isDashing = false;
			pBody->body->SetLinearVelocity({ 0,0 });
			new ParticleTeleport(GetPosition());
		}

		// Alpha animation------------------
		if (renderObjects[0].color.a > 0) DashAnimation(-70);
		else if (playerShadow->enable)
		{
			playerShadow->enable = false;
		}
	}
	else
	{	
		// Alpha animation------------------
		if (renderObjects[0].color.a < 255) DashAnimation(70);
		else if (!playerShadow->enable)
		{
			playerShadow->enable = true;
		}
	}

	if (isAttackImpulse)
	{
		attackImpulseCounter -= playerTimer.getDeltaTime() * 1000;
		// If Cooldown is done, you stop dashing
		if (attackImpulseCounter <= 0)
		{
			isAttackImpulse = false;
		}
	}

	playerTimer.Reset();

	if (!isInvulnerable && beenHit)beenHit = false;

	// Every frame set the linear velocity to 0 in case we are not moving and we are not dashing
	// This is done to prevent drifting when applying forces from other bodies into the player body.
	if (!isDashing)pBody->body->SetLinearVelocity(b2Vec2(0, 0));

	// If player control is available
	if (canControl)
	{
		// Get Movement Input
		if (app->input->usingGameController) MovementUpdateController();
		else MovementUpdateKeyboard();

		// Get Combat Input
		combat->CombatUpdate();

	}
	
	// If our current animation has finished, we reset it manually. This is made for DASH and ATTACK animations.
	// When these animations end, you must Reset them for the next time you'll use them
	if (animations[(int)currentAnim].HasFinished()) animations[(int)currentAnim].Reset();
}

void PlayerController::Update()
{
	if (pendingToDelete) return;
	// Update State Machine
	stateMachine.Update();

	// Update current player State
	currentState = (PlayerState)stateMachine.GetCurrentState();

	// Update animation
	animations[(int)currentAnim].Update();	


	//GodMode Active
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && !godMode) {
		godMode = !godMode;
		normalDamage = player->damage;
		player->damage = 1000;
		
	}
	else if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && godMode) {
		godMode = !godMode;
		player->damage = normalDamage;
	}

	//LOG("Damage: %d", player->damage);
}

void PlayerController::PostUpdate()
{
	if (pendingToDelete) return;

	if (invulnerabilityCounter % 10 == 1 && beenHit) return;
	// Update current Animation state 
	
	SetAnimationState(); // Sets the current animation to be drawed

	/*if (godMode) renderObjects[0].SetColor({ 0, 255, 17 ,255 });
	else  renderObjects[0].SetColor({ 255, 255, 255 ,255 });*/

	UpdateOrderInLayer(0);
	renderObjects[0].section = animations[(int)currentAnim].GetCurrentFrame();
	renderObjects[0].destRect.x = GetDrawPosition().x + textureOffset.x;
	renderObjects[0].destRect.y = GetDrawPosition().y + textureOffset.y;

	if (lookingDir == LookingDirection::LEFT) renderObjects[0].flip = SDL_FLIP_HORIZONTAL;
	else if (lookingDir == LookingDirection::RIGHT) renderObjects[0].flip = SDL_FLIP_NONE;

	app->renderer->AddRenderObjectRenderQueue(renderObjects[0]);
}

void PlayerController::CleanUp()
{
	app->events->RemoveListener(this);
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
	tryingToMove = false;
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
		tryingToMove = true;
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
		tryingToMove = true;
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
		tryingToMove = true;
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
		tryingToMove = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
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

	// If we are not in running state, we undo any velocity changes into our physic body.
	if (stateMachine.GetCurrentState() != (uint)PlayerState::RUN && !isAttackImpulse) pBody->body->SetLinearVelocity({0,0});

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (!isDashing)
		{
			//Reset Dash animation in case it hadn't finished yet
			animations[(int)PlayerAnim::DASH_HORIZONTAL].Reset();
			animations[(int)PlayerAnim::DASH_UP].Reset();
			animations[(int)PlayerAnim::DASH_DOWN].Reset();

			//Change Player State
			stateMachine.ChangeState((uint)PlayerState::DASH);

			// do the dash
			DashOn();
			app->audio->PlayFx(playerdodgeFX);
		}
	}
}

void PlayerController::MovementUpdateController()
{
	tryingToMove = false;
	// By default, the player is always IDLE
	stateMachine.ChangeState((uint)PlayerState::IDLE);

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
			tryingToMove = true;
			lookingDir = LookingDirection::RIGHT;
		}
		else if (direction.x < 0)
		{
			stateMachine.ChangeState((uint)PlayerState::RUN);
			tryingToMove = true;
			lookingDir = LookingDirection::LEFT;
		}
	}
	else
	{
		if (direction.y < 0)
		{
			stateMachine.ChangeState((uint)PlayerState::RUN);
			tryingToMove = true;
			lookingDir = LookingDirection::UP;
		}
		else if (direction.y > 0)
		{
			//Change Player State
			stateMachine.ChangeState((uint)PlayerState::RUN);
			tryingToMove = true;
			lookingDir = LookingDirection::DOWN;
		}
	}

	// Add speed to direction vector
	direction.x *= speed;
	direction.y *= speed;
	
	// Apply speed if we are currently running (so it doesnt apply when attacking)
	if (stateMachine.GetCurrentState() == (uint)PlayerState::RUN) pBody->body->SetLinearVelocity(direction);

	
	if (app->input->GetControllerButton(BUTTON_B) == KEY_DOWN)
	{
		if (!isDashing)
		{
			//Reset Dash animation in case it hadn't finished yet
			animations[(int)PlayerAnim::DASH_HORIZONTAL].Reset();
			animations[(int)PlayerAnim::DASH_UP].Reset();
			animations[(int)PlayerAnim::DASH_DOWN].Reset();

			//Change Player State
			stateMachine.ChangeState((uint)PlayerState::DASH);

			// do the dash
			DashOn();

			// Disable Attack Impulse
			attackImpulseCounter = 0;
		}
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

	new ParticleTeleport(GetPosition());
}

void PlayerController::DashAnimation(int sum)
{
	int currentAlpha = renderObjects[0].color.a + sum;

	if (currentAlpha < 0) currentAlpha = 0;
	if (currentAlpha > 255) currentAlpha = 255;

	renderObjects[0].color.a = currentAlpha;
}

void PlayerController::SetAnimationState()
{
	switch (currentState)
	{
	case PlayerState::IDLE:		// There is no IDLE sprite, so we just change the current animation to the run animation.
	case PlayerState::RUN:
		if (lookingDir == LookingDirection::DOWN)
		{
			currentAnim = PlayerAnim::RUN_DOWN;
		}
		if (lookingDir == LookingDirection::LEFT || lookingDir == LookingDirection::RIGHT)
		{
			currentAnim = PlayerAnim::RUN_HORIZONTAL;
		}
		if (lookingDir == LookingDirection::UP)
		{
			currentAnim = PlayerAnim::RUN_UP;
		}
		
		break;
	case PlayerState::ATTACK:
		currentAnim == PlayerAnim::IDLE;
		break;
	case PlayerState::DASH:
		if (lookingDir == LookingDirection::LEFT || lookingDir == LookingDirection::RIGHT) currentAnim = PlayerAnim::DASH_HORIZONTAL;
		else if (lookingDir == LookingDirection::UP) currentAnim = PlayerAnim::DASH_UP;
		else currentAnim == PlayerAnim::DASH_DOWN;
		break;
	}
}

fPoint PlayerController::GetPlayerToMouseVector()
{
	fPoint vec = { (float)(app->input->GetMouseX() - GetScreenPosition().x), (float)(app->input->GetMouseY() - GetScreenPosition().y ) };
	vec = vec.Normalize();

	return vec;
}

void PlayerController::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject == nullptr) return;
	
	if (col->gameObject->CompareTag("HubDoorIN"))
	{
		app->map->roof = false;
	}

	if (col->gameObject->CompareTag("HubDoorOUT"))
	{
		app->map->roof = true;
	}
}

void PlayerController::OnCollisionExit(PhysBody* col)
{

}

void PlayerController::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (isInvulnerable) return;

	if (col->gameObject->name == "DamageArea")
	{
		DamageArea* dArea = (DamageArea*)col->gameObject;
		Hit(dArea->GetDamage());
	    Stun(dArea->stunTime);
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
	if (!godMode) 
	{
		app->audio->PlayFx(playerhitFX);
		int totalDamage = damage - player->shield;
		if (totalDamage < 0) totalDamage = 0;
		player->ChangeShield(-damage);

		printf("Player beefore HP:%d\t", player->hpPlayer);

		player->hpPlayer -= totalDamage;

		printf("Player after HP:%d\n", player->hpPlayer);

		if (player->hpPlayer <= 0)
		{
			enemyTrigger->pendingToDelete = true;

			enemyTrigger = nullptr;
			/*app->scene->ChangeCurrentSceneRequest(SCENES::GAME_OVER, 60);*/

			app->events->TriggerEvent(GameEvent::PLAYER_DIE);
			app->events->TriggerEvent(GameEvent::SAVE_GAME_LOSE);

			canControl = false;

			return;
		}

		// Camera Shake effect
		app->renderer->camera->Shake(30, 10, 2);

		beenHit = true;
		Invulnerability(invulnerabilityTimeHit);

		app->events->TriggerEvent(GameEvent::PLAYER_HIT);
	}
}

void PlayerController::Stun(int frames)
{
}

void PlayerController::Invulnerability(int frames)
{
	isInvulnerable = true;
	invulnerabilityCounter = frames;
}

void PlayerController::GameEventTriggered(GameEvent id)
{
	switch (id)
	{
	case GameEvent::PLAYER_DIE:
		app->scene->ChangeCurrentSceneRequest(SCENES::GAME_OVER, 20);
		break;
	case GameEvent::COMPLETE_ROOM:
		combat->CheckDeck();
		break;
	case GameEvent::STOP_PLAYER_MOVEMENT:
		canControl = false;
		break;
	case GameEvent::RESUME_PLAYER_MOVEMENT:
		canControl = true;
		break;
	case GameEvent::ENEMY_HIT:
		purifiedSwordHeals();
		break;
	}
}

void PlayerController::AttackImpulse()
{
	isAttackImpulse = true;
	attackImpulseCounter = attackImpulseTime;

	b2Vec2 dir;
	//We dash on the current direction we are facing
	switch (lookingDir)
	{
	case LookingDirection::UP:
		dir = { 0, (float)-1 * attackImpulseDistance };
		break;
	case LookingDirection::DOWN:
		dir = { 0, (float)1 * attackImpulseDistance };
		break;
	case LookingDirection::LEFT:
		dir = { (float)-1 * attackImpulseDistance, 0 };
		break;
	case LookingDirection::RIGHT:
		dir = { (float)1 * attackImpulseDistance, 0 };
		break;
	}

	pBody->body->SetLinearVelocity(dir);
}

void PlayerController::purifiedSwordHeals()
{
	int heal = 5;
	int playerLife = player->hpPlayer;
	if (player->purifiedSwordOn)
	{
		/*if (playerLife >= player->hpMax)
		{
			
			return;
		}*/
		if ((playerLife + heal) > player->hpMax)
		{
			player->hpPlayer = player->hpMax;
		}
		else
		{
			player->hpPlayer += heal;
		}
	}
	app->events->TriggerEvent(GameEvent::UPDATE_PLAYER_HP);
}

