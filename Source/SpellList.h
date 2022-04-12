#ifndef _SPELL_LIST_
#define _SPELL_LIST_

#include "Spell.h"
#include "Globals.h"

#define TOTAL_SPELLS 11 // Total Spells in the game, including the NONE one

// This class contains every spell on the game. It gets an instance when the player is created on scene.
// Spell information is gathered from here. 
// Any new spell must be added to the spells array when created.

class SpellList
{
private:
	static SpellList* instance;


public:

	static SpellList* GetInstance();

	Spell* spells[TOTAL_SPELLS];

	void Update();

	void ReleaseInstance();


private:

	SpellList();

	~SpellList();

	void CleanUp();
};

#endif // !_SPELL_LIST_