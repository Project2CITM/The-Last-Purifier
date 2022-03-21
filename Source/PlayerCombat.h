#ifndef _PLAYER_COMBAT_
#define _PLAYER_COMBAT_

#include "Globals.h"
#include "List.h"
#include "SpellList.h"
#include "ExecuteSpell.h"

class PlayerCombat
{
public:
	PlayerCombat();

	void Attack();

	void CastSpell();

	void ChangeSelectedSpellSlot(int num);

	void CleanUp();

	int availableSpellSlots; // Determines how many available spell slots the player has.

private:
	List<SpellID> spellSlots; // A list of every spell slot and its current spell. If empty, it contains NONE.
	int selectedSpell; // The current selected spell slot on the list. This is an index.
	ExecuteSpell executeSpellCommand; // The command that fires the selected spell on the current spell slot to the SpellList.

};

#endif