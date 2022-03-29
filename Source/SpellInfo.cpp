#include "SpellInfo.h"
#include "SpellList.h"

SpellInfo::SpellInfo(SpellID id, uint level)
{
	this->id = id;
	this->spellLevel = level;
	this->uses = SpellList::GetInstance()->spells[(int)id]->GetUses(); //Get spell ID's spell uses
}

void SpellInfo::ChangeSpell(SpellID id, uint level)
{
	this->id = id;
	this->spellLevel = level;
	this->uses = SpellList::GetInstance()->spells[(int)id]->GetUses();
}

void SpellInfo::EmptySpell()
{
	id = SpellID::NONE;
	spellLevel = 1;
	uses = -1;
}
