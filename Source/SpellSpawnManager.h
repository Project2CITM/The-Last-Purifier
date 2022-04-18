#ifndef _SPELL_SPAWN_MANAGER_
#define _SPELL_SPAWN_MANAGER_

#include "Point.h"
#include "ClassTree.h"
#include "Spell.h"

class SpellSpawnManager
{
public:
	static SpellSpawnManager* GetInstance();

	void SpawnSpell(iPoint p);

	void ReleaseInstance();

	bool IsDeleted();

private:
	SpellSpawnManager(PlayerClass playerClass);

	int CalculateSpellLevel(SpellID id);

	static SpellSpawnManager* instance;

	ClassTree* classTree = nullptr;

	PlayerClass pClass;

	int revenantSpells = 3;
	int sageSpells = 2;

	int currentRevenantSpells[3] = { 1,2,6 };
	int currentSageSpells[2] = { 11, 14 };

	int fakepoints = 1000000;

	bool deleted = false;
};
#endif

