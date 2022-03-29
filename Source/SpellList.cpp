#include "SpellList.h"
#include "TestSpell.h"

SpellList* SpellList::instance = nullptr;

SpellList* SpellList::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new SpellList();
	}
	return instance;
}

SpellList::SpellList()
{
	spells[(int)SpellID::NONE] = new Spell();
	spells[(int)SpellID::CREMATORIUM] = new TestSpell();
	spells[(int)SpellID::DASH_SLASH] = new TestSpell();
	spells[(int)SpellID::FLAME_CAPE] = new TestSpell();
	spells[(int)SpellID::PURIFICATION_SLASH] = new TestSpell();
	spells[(int)SpellID::PURIFICATION_THRUST] = new TestSpell();
	spells[(int)SpellID::PURIFIED_SWORD] = new TestSpell();
	spells[(int)SpellID::SACRED_SHIELD] = new TestSpell();
	spells[(int)SpellID::SOUL_SHIELD] = new TestSpell();
	spells[(int)SpellID::TORMENT] = new TestSpell();
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
