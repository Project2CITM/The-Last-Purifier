#include "PlayerCombat.h"
#include "Player.h"
#include "ModulePhysics.h"
#include "PlayerController.h"
#include "SpellInfo.h"
#include "Projectile.h"
#include "ModuleEvents.h"
#include "ParticleAttackRevenant.h"

PlayerCombat::PlayerCombat(std::string name, std::string tag, Player* player) : GameObject(name, tag)
{
	this->player = player;
}

void PlayerCombat::Start()
{
	// Change filter
	b2Filter filter;

	filter.categoryBits = app->physics->PLAYER_LAYER;

	revenantAttack = new DamageArea(player->controller->GetPosition(), 15, 20, player->damage);

	revenantAttack->pBody->body->GetFixtureList()->SetFilterData(filter);

	revenantAttack->pBody->body->SetActive(false);

	executeSpellCommand = new ExecuteSpell();

	// Attack action stats
	attackCD = player->attackSpeed;
	attackCounter = 0;
	canAttack = true;

	// Attack area stats
	attackAreaActive = false;
	attackAreaCD = 5;
	attackAreaCounter = 0;

	// Initialize available spell slots and deck slots
	availableSpellSlots = player->spellSlots;
	availableDeckSlots = player->deckSlots;

	for (int i = 0; i < availableSpellSlots; i++)
	{
		spellSlots.add(new SpellInfo());
	}

	for (int i = 0; i < availableDeckSlots; i++)
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
}

void PlayerCombat::Update()
{
	if (!canAttack)
	{
		attackCounter++;
		if (attackCounter >= attackCD)
		{
			canAttack = true;
			attackCounter = 0;
		}
	}

	if (attackAreaActive)
	{
		attackAreaCounter++;
		if (attackAreaCounter >= attackAreaCD)
		{
			attackAreaActive = false;
			revenantAttack->pBody->body->SetActive(false);
			attackAreaCounter = 0;
		}
	}
}

void PlayerCombat::Attack()
{
	if (!canAttack) return;

	switch (player->playerClass)
	{
	case PlayerClass::REVENANT:
		RevenantAttack();
		printf("Attack Revenant!\n");
		break;
	case PlayerClass::SAGE:
		printf("Attack Sage!\n");
		SageAttack();
		break;
	}
	app->events->TriggerEvent(GameEvent::PLAYER_ATTACK);
	canAttack = false;
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

	if (selectedSpell >= availableSpellSlots) // If we are over the limit, go back to first
	{
		selectedSpell = 0;
	}
	else if (selectedSpell < 0) // If we are under the first, go to the last slot
	{
		selectedSpell = availableSpellSlots - 1;
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
			}
		}
		
	}
}

void PlayerCombat::CleanUp()
{
	if (pendingToDelete) revenantAttack->pendingToDelete = true;
	
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

void PlayerCombat::RevenantAttack()
{
	// Set area as active
	revenantAttack->pBody->body->SetActive(true);

	// Calculate attack offset and rotation based on looking direction
	b2Vec2 attackOffset = GetAttackOffset();
	float attackRotation = 0;
	if (attackOffset.x == 0.25f) attackRotation = 90 * DEGTORAD;
	
	// Update revenantAttack
	revenantAttack->damage = player->damage + player->extraDamage;
	revenantAttack->stunTime = player->stunTime;
	revenantAttack->pushDistance = player->pushDistance;

	// Place on correct position
	revenantAttack->pBody->body->SetTransform(player->controller->pBody->body->GetPosition() + attackOffset, attackRotation);

	iPoint particleOffset;
	int particleRotation = 0;
	switch (player->controller->lookingDir)
	{
	case LookingDirection::DOWN:
		particleOffset = { -25, -30 };
		particleRotation = 90;
		break;
	case LookingDirection::UP:
		particleOffset = { -25, -20 };
		particleRotation = 270;
		break;
	case LookingDirection::LEFT:
		particleOffset = {-20, -25 };
		particleRotation = 180;
		break;
	case LookingDirection::RIGHT:
		particleOffset = { -30, -25 };
		particleRotation = 0;
		break;

	}

	new ParticleAttackRevenant(revenantAttack->GetPosition() + particleOffset, particleRotation, 0.15f, 0, player->purifiedSwordOn);

	attackAreaActive = true;
}

void PlayerCombat::SageAttack()
{
	iPoint attackOffset = { METERS_TO_PIXELS(GetAttackOffset().x), METERS_TO_PIXELS(GetAttackOffset().y) };

	// Get projectile speed
	fPoint speed = { 0,0 };
	iPoint particleOffset;
	int particleRotation = 0;
	switch (player->controller->lookingDir)
	{
	case LookingDirection::UP:
		speed.y = -1;
		particleRotation = 270;
		break;
	case LookingDirection::DOWN:
		speed.y = 1;
		particleRotation = 90;
		break;
	case LookingDirection::LEFT:
		speed.x = -1;
		particleRotation = -180;
		break;
	case LookingDirection::RIGHT:
		speed.x = 1;
		particleRotation = 0;
		break;
	}

	new Projectile("Projectile", player->controller->GetPosition() + attackOffset, speed * projectileSpeed,player->damage,particleRotation);
}
