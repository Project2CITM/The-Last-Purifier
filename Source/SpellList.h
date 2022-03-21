#ifndef _SPELL_LIST_
#define _SPELL_LIST_

#include "Spell.h"
#include "Globals.h"

#define TOTAL_SPELLS 11 // Total Spells in the game, including the NONE one

// This class contains every spell on the game. It gets an instance when the player is created on scene.
// Spell information is gathered from here. 
// Any new spell must be added to the spells array when created.

enum class SpellID
{
	NONE = 0,
	PURIFIED_SWORD,
	SOUL_SHIELD,
	FLAME_CAPE,
	SACRED_SHIELD,
	COVER,
	PURIFICATION_SLASH,
	PURIFICATION_THRUST,
	DASH_SLASH,
	TORMENT,
	CREMATORIUM
};

class SpellList
{
public:
	SpellList();

	Spell* spells[TOTAL_SPELLS];

	void Update();

	void CleanUp();
};

#endif // !_SPELL_LIST_