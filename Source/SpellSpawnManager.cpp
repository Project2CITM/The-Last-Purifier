#include "SpellSpawnManager.h"
#include "Player.h"
#include "SpellObject.h"

SpellSpawnManager* SpellSpawnManager::instance = nullptr;

SpellSpawnManager* SpellSpawnManager::GetInstance(PlayerClass playerClass)
{
    if (instance == nullptr)
    {
        instance = new SpellSpawnManager(playerClass);
    }

    return instance;
}

void SpellSpawnManager::SpawnSpell(iPoint position)
{
    int randomSpell;
    if (pClass == PlayerClass::REVENANT) randomSpell = rand() % revenantSpells + 1;
    else randomSpell = (rand() % revenantSpells + 1) + sageSpells;

    SpellObject* spellObject = new SpellObject(position, (SpellID)randomSpell, CalculateSpellLevel((SpellID)randomSpell));
}

SpellSpawnManager::SpellSpawnManager(PlayerClass playerClass)
{
    pClass = playerClass;
    classTree = ClassTree::GetInstance(playerClass);
}

void SpellSpawnManager::ReleaseInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

int SpellSpawnManager::CalculateSpellLevel(SpellID id)
{
    int totalLevels = classTree->getCurrentLevel((int)id);
    int level = 1;

    int result = rand() % 100 + 1;
    switch (totalLevels)
    {
    case 2:
        if (result > 50) level = 2;
        break;
    case 3:
        if (result <= 50) break;
        else if (result <= 85) level = 2;
        else level = 3;
    default:
        break;
    }

    return level;
}
