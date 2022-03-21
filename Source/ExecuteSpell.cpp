#include "ExecuteSpell.h"

ExecuteSpell::ExecuteSpell()
{
}

bool ExecuteSpell::Execute(SpellID spell)
{
	return spellList.spells[(int)spell]->Execute();
}

void ExecuteSpell::CleanUp()
{
	spellList.CleanUp();
}
