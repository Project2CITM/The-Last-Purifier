#include "PlayerCombat.h"
#include "Player.h"
#include "ModulePhysics.h"
#include "PlayerController.h"
#include "SpellInfo.h"
#include "Projectile.h"
#include "ModuleEvents.h"
#include "ModuleAudio.h"
#include "RevenantSword.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "SageWeapon.h"
#include "SageStaff.h"
#include "RevenantSpear.h"

PlayerCombat::PlayerCombat(std::string name, std::string tag, Player* player) : GameObject(name, tag)
{
	this->player = player;
	this->listenTo[0] = GameEvent::UPDATE_PLAYER_STATS;
	app->events->AddListener(this);

	revenantWeapon = new RevenantSpear(this->player->controller);
	sageWeapon = new  SageStaff(this->player->controller);

	SetClassWeaponCD();

}

void PlayerCombat::Start()
{
	executeSpellCommand = new ExecuteSpell();

	for (int i = 0; i < player->spellSlots; i++)
	{
		spellSlots.add(new SpellInfo());
	}

	for (int i = 0; i < player->deckSlots; i++)
	{
		deckSlots.add(new SpellInfo());
	}

	selectedSpell = 0;
}

void PlayerCombat::PreUpdate()
{
	if (!isSpellCommandStarted) 
	{
		executeSpellCommand->Start();
	}
	// Clas weapon preupdate
	if (player->playerClass == PlayerClass::REVENANT) revenantWeapon->PreUpdate();
	else sageWeapon->PreUpdate();
}

void PlayerCombat::Update()
{
	executeSpellCommand->Update();
}

void PlayerCombat::CombatUpdate()
{
	combatTimer.Update();

	if (app->input->GetMouseButton(1) == KEY_UP || app->input->GetControllerButton(BUTTON_X) == KEY_UP)
	{
		Attack(chargedAttackTime);
		chargedAttackTime = 0;

		// Update charging bar
		if (player->playerClass == PlayerClass::SAGE) sageWeapon->UpdateAttackBar(false);
	}
 
	// Check for attack and Spell input
	if (app->input->GetMouseButton(1) == KEY_REPEAT || app->input->GetControllerButton(BUTTON_X) == KEY_REPEAT)
	{
		chargedAttackTime += combatTimer.getDeltaTime() * 1000;

		// Update charging bar
		if (player->playerClass == PlayerClass::SAGE) sageWeapon->UpdateAttackBar(true, combatTimer.getDeltaTime() * 1000);
	}
	else if (app->input->GetMouseButton(3) == KEY_DOWN || app->input->GetControllerButton(BUTTON_A) == KEY_DOWN)
	{
		CastSpell();
	}

	// Check for spell changing input

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN || app->input->GetControllerButton(BUTTON_LEFT_SHOULDER) == KEY_DOWN)
	{
		ChangeSelectedSpellSlot(-1);
	}
	else if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetControllerButton(BUTTON_RIGHT_SHOULDER) == KEY_DOWN)
	{
		ChangeSelectedSpellSlot(1);
	}

	combatTimer.Reset();
}

void PlayerCombat::Attack(int chargedTime)
{
	switch (player->playerClass)
	{
	case PlayerClass::REVENANT:
		// If the weapon is ready, the player can attack and the player isn't already attacking
		if (revenantWeapon->canAttack && player->controller->stateMachine.GetCurrentState() != (uint)PlayerState::ATTACK 
			&& player->controller->stateMachine.ChangeState((uint)PlayerState::ATTACK))
		{
			revenantWeapon->Attack(chargedTime); // Attack and trigger attack event.
			if (player->controller->tryingToMove && revenantWeapon->addImpulse)
			{
				player->controller->AttackImpulse(); // Move when attacking if player is trying to move
			}
			app->events->TriggerEvent(GameEvent::PLAYER_ATTACK);
			app->renderer->camera->Shake(5, 10, 2);
		}
		

		break;
	case PlayerClass::SAGE:

		if (sageWeapon->Attack(chargedTime))
		{
			app->events->TriggerEvent(GameEvent::PLAYER_ATTACK);
			app->renderer->camera->Shake(5, 10, 2);
			player->controller->stateMachine.ChangeState((uint)PlayerState::ATTACK);
		};
		break;
	}

}

void PlayerCombat::CastSpell()
{
	if (executeSpellCommand->Execute(spellSlots[selectedSpell])) // Execute the selected spell and get response
	{ 
		// If returns true, the selected spell is deleted from the current spell slot
		spellSlots[selectedSpell]->EmptySpell();
	}
}

void PlayerCombat::ChangeSelectedSpellSlot(int num)
{
	selectedSpell += num; // Change Selected Spell Slot

	if (selectedSpell >= player->spellSlots) // If we are over the limit, go back to first
	{
		selectedSpell = 0;
	}
	else if (selectedSpell < 0) // If we are under the first, go to the last slot
	{
		selectedSpell = player->spellSlots - 1;
	}

	printf("Current Spell Slot: %d\n", selectedSpell);
}

void PlayerCombat::StartExecuteSpellCommand()
{
	executeSpellCommand->Start();
	isSpellCommandStarted = true;
}

bool PlayerCombat::AddSpell(SpellInfo spell)
{
	// Check for an empty spell slot.
	for (int i = 0; i < spellSlots.count(); i++)
	{
		if (spellSlots[i]->id == SpellID::NONE)
		{
			spellSlots[i]->ChangeSpell(spell.id, spell.spellLevel);
			PrintSlotsState();
			return true;
		}
	}

	// Check for an empty deck slot.
	for (int i = 0; i < deckSlots.count(); i++)
	{
		if (deckSlots[i]->id == SpellID::NONE)
		{
			deckSlots[i]->ChangeSpell(spell.id, spell.spellLevel);
			PrintSlotsState();
			return true;
		}
	}

	// If neither the spell slots nor the deck slots have an empty slot, return false
	return false;
}

bool PlayerCombat::ChangeRevenantWeapon(RevenantWeaponIDs id)
{
	// After changing the current weapon you must change the player Attack State time.
	SetClassWeaponCD();
	return true;
}

bool PlayerCombat::ChangeSageWeapon(SageWeaponIDs id)
{	
	// After changing the current weapon you must change the player Attack State time.
	SetClassWeaponCD();
	return true;
}

void PlayerCombat::SetClassWeaponCD()
{
	if (this->player->playerClass == PlayerClass::REVENANT) player->controller->stateMachine.states[(uint)PlayerState::ATTACK].totalTime = revenantWeapon->attackSpeedCD;
	else player->controller->stateMachine.states[(uint)PlayerState::ATTACK].totalTime = sageWeapon->attackSpeedCD;
}

void PlayerCombat::CheckDeck()
{
	for (int i = 0; i < spellSlots.count(); i++)
	{
		// If the spell slot is not empty, check the next one
		if (spellSlots[i]->id != SpellID::NONE) continue;
		
		// If a spell slot is empty, fill it with one of the deck Slots.
		for (int j = 0; j < deckSlots.count(); j++)
		{
			if (deckSlots[j]->id != SpellID::NONE) // Not empty deck slot
			{
				spellSlots[i]->ChangeSpell(deckSlots[j]->id, deckSlots[j]->spellLevel);
				deckSlots[j]->EmptySpell();
				break;
			}
		}
		
	}
}

fPoint PlayerCombat::GetProjectileOffset()
{
	// Get projectile speed
	fPoint duration = { 0,0 };
	switch (player->controller->lookingDir)
	{
	case LookingDirection::UP:
		duration.y = -1;
		break;
	case LookingDirection::DOWN:
		duration.y = 1;
		break;
	case LookingDirection::LEFT:
		duration.x = -1;
		break;
	case LookingDirection::RIGHT:
		duration.x = 1;
		break;
	}

	return duration;

}

int PlayerCombat::GetProjectileRotation()
{
	int particleRotation = 0;
	switch (player->controller->lookingDir)
	{
	case LookingDirection::DOWN:
		particleRotation = 90;
		break;
	case LookingDirection::UP:
		particleRotation = 270;
		break;
	case LookingDirection::LEFT:
		particleRotation = 180;
		break;
	case LookingDirection::RIGHT:
		particleRotation = 0;
		break;
	}
	return particleRotation;
}


void PlayerCombat::CleanUp()
{
	if (revenantWeapon != nullptr)
	{
		revenantWeapon->CleanUp();
		RELEASE(revenantWeapon);
	}
	if (sageWeapon != nullptr)
	{
		sageWeapon->CleanUp();
		RELEASE(sageWeapon);
	}

	app->events->RemoveListener(this);
	executeSpellCommand->CleanUp();
	RELEASE(executeSpellCommand);
	spellSlots.clearPtr();
	deckSlots.clearPtr();
}

void PlayerCombat::PrintSlotsState()
{
	for (int i = 0; i < spellSlots.count(); i++)
	{
		printf("Spell slot %d contains spell %d at level %d\n", i, (int)spellSlots[i]->id, spellSlots[i]->spellLevel);
	}
	for (int i = 0; i < deckSlots.count(); i++)
	{
		printf("Deck slot %d contains spell %d at level %d\n", i, (int)deckSlots[i]->id, deckSlots[i]->spellLevel);
	}
}

void PlayerCombat::GameEventTriggered(GameEvent id)
{
	spellSlots.clearPtr();
	deckSlots.clearPtr();

	for (int i = 0; i < player->spellSlots; i++)
	{
		spellSlots.add(new SpellInfo());
	}

	for (int i = 0; i < player->deckSlots; i++)
	{
		deckSlots.add(new SpellInfo());
	}

	selectedSpell = 0;
}

b2Vec2 PlayerCombat::GetAttackOffset()
{
	b2Vec2 attackOffset = { 0,0 };
	switch (player->controller->lookingDir)
	{
	case LookingDirection::DOWN:
		attackOffset = { 0.25f, 0.75f };
		break;
	case LookingDirection::UP:
		attackOffset = { 0.25f, -2.0f };
		break;
	case LookingDirection::LEFT:
		attackOffset = { -0.75f, -0.75f };
		break;
	case LookingDirection::RIGHT:
		attackOffset = { 1.5f, -0.75f };
		break;
	}

	return attackOffset;
}

