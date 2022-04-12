#include "ExecuteSpell.h"
#include "SpellInfo.h"

ExecuteSpell::ExecuteSpell()
{
	spellList = SpellList::GetInstance();
}

bool ExecuteSpell::Execute(SpellInfo* spell)
{
	spellList->spells[(int)spell->id]->Execute(spell->spellLevel);

	return --spell->uses == 0;
}

void ExecuteSpell::CleanUp()
{
	spellList->ReleaseInstance();
}
