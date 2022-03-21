#include "PlayerCombat.h"

PlayerCombat::PlayerCombat()
{
	// Test Code-----------------------------------------
	availableSpellSlots = 4; 

	for (int i = 0; i < availableSpellSlots; i++)
	{
		spellSlots.add(SpellID::NONE);
	}

	spellSlots[0] = SpellID::COVER;

	// Test Code-----------------------------------------

	selectedSpell = 0;
}

void PlayerCombat::Attack()
{
	printf("Attack!\n");
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

void PlayerCombat::CleanUp()
{
	executeSpellCommand.CleanUp();
	spellSlots.clear();
}
