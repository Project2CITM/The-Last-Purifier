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

	int revenantSpells = 10;
	int sageSpells = 9;

	int fakepoints = 1000000;

	bool deleted = false;
};
#endif

