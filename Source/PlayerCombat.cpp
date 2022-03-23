#include "PlayerCombat.h"
#include "Player.h"

PlayerCombat::PlayerCombat(Player* player)
{
	this->player = player;

	// Test Code-----------------------------------------
	availableSpellSlots = 4; 

	availableDeckSlots = 6;

	for (int i = 0; i < availableSpellSlots; i++)
	{
		spellSlots.add(SpellID::NONE);
	}

	for (int i = 0; i < availableDeckSlots; i++)
	{
		deckSlots.add(SpellID::NONE);
	}

	spellSlots[0] = SpellID::COVER;

	// Test Code-----------------------------------------

	selectedSpell = 0;
}

void PlayerCombat::Attack()
{
	switch (player->playerClass)
	{
	case PlayerClass::REVENANT:
		printf("Attack Revenant!\n");
		break;
	case PlayerClass::SAGE:
		printf("Attack Sage!\n");
		break;
	}
}

void PlayerCombat::CastSpell()
{
	if (executeSpellCommand.Execute(spellSlots[selectedSpell])) // Execute the selected spell and get response
	{ 
		// If returns true, the selected spell is deleted from the current spell slot
		spellSlots[selectedSpell] = SpellID::NONE;
	}
}

void PlayerCombat::ChangeSelectedSpellSlot(int num)
{
	selectedSpell += num; // Change Selected Spell Slot

	if (selectedSpell > availableSpellSlots) // If we are over the limit, go back to first
	{
		selectedSpell = 0;
	}
	else if (selectedSpell < 0) // If we are under the first, go to the last slot
	{
		selectedSpell = availableSpellSlots - 1;
	}

	printf("Current Spell Slot: %d\n", selectedSpell);


}

bool PlayerCombat::AddSpell(SpellID spell)
{
	// Check for an empty spell slot.
	for (int i = 0; i < spellSlots.count(); i++)
	{
		if (spellSlots[i] == SpellID::NONE)
		{
			spellSlots[i] = spell;
			PrintSlotsState();
			return true;
		}
	}

	// Check for an empty deck slot.
	for (int i = 0; i < deckSlots.count(); i++)
	{
		if (deckSlots[i] == SpellID::NONE)
		{
			deckSlots[i] = spell;
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
		if (spellSlots[i] != SpellID::NONE) continue;
		
		// If an spell slot is empty, fill it with one of the deck Slots.
		for (int j = 0; j < deckSlots.count(); j++)
		{
			if (deckSlots[j] != SpellID::NONE) // Not empty deck slot
			{
				spellSlots[i] = deckSlots[j]; // Fill spell slot and empty deck slot

				deckSlots[j] = SpellID::NONE;
			}
		}
		
	}
}

void PlayerCombat::CleanUp()
{
	executeSpellCommand.CleanUp();
	spellSlots.clear();
}

void PlayerCombat::PrintSlotsState()
{
	for (int i = 0; i < spellSlots.count(); i++)
	{
		printf("Spell slot %d contains spell %d\n", i, (int)spellSlots[i]);
	}
	for (int i = 0; i < deckSlots.count(); i++)
	{
		printf("Deck slot %d contains spell %d\n", i, (int)deckSlots[i]);
	}
}
