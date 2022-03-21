#include "SpellList.h"
#include "TestSpell.h"

SpellList::SpellList()
{
	spells[(int)SpellID::NONE] = new Spell();
	spells[(int)SpellID::COVER] = new TestSpell();
}

void SpellList::Update()
{
	for (int i = 0; i < TOTAL_SPELLS; i++)
	{
		if (spells[i] != nullptr)
		{
			spells[i]->Update();
		}
	}
}

void SpellList::CleanUp()
{
	for (int i = 0; i < TOTAL_SPELLS; i++)
	{
		if (spells[i] != nullptr)
		{
			spells[i]->CleanUp();
			RELEASE(spells[i]);
		}
	}
}
