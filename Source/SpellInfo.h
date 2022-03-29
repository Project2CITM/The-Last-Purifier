#ifndef _SPELL_INFO_
#define _SPELL_INFO_

#include "Spell.h"
#include "Globals.h"

// This class contains all information relative to a spell that conerncs the player or SpellObjects (collectable items that contain spells)
class SpellInfo
{
public:
	SpellID id;
	uint spellLevel;
	int uses;

	SpellInfo(SpellID id = SpellID::NONE, uint level = 1);

	void ChangeSpell(SpellID id, uint level);
	void EmptySpell();

};
#endif

