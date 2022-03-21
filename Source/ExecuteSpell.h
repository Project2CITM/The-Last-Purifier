#ifndef _EXECUTE_SPELL_
#define _EXECUTE_SPELL_

#include "SpellList.h"

class ExecuteSpell
{
public:
	ExecuteSpell();

	bool Execute(SpellID spell);

	void CleanUp();

private:
	SpellList spellList;
};

#endif

