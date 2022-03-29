#ifndef _EXECUTE_SPELL_
#define _EXECUTE_SPELL_

#include "SpellList.h"

class SpellInfo;

class ExecuteSpell
{
public:
	ExecuteSpell();

	bool Execute(SpellInfo* spell);

	void CleanUp();

private:
	SpellList* spellList;
};

#endif

