#include "ExecuteSpell.h"
#include "SpellInfo.h"

ExecuteSpell::ExecuteSpell()
{
	
}

void ExecuteSpell::Start()
{
	spellList = SpellList::GetInstance();
}

void ExecuteSpell::Update()
{
	if (spellList != nullptr) spellList->Update();
}

bool ExecuteSpell::Execute(SpellInfo* spell)
{
	spellList->spells[(int)spell->id]->Execute(spell->spellLevel);

	return --spell->uses == 0;
}

void ExecuteSpell::CleanUp()
{
	if (spellList == nullptr) return;

	spellList->ReleaseInstance();
}
