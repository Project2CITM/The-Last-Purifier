#include "SpellSpawnManager.h"
#include "Player.h"
#include "SpellObject.h"
#include "SceneGame.h"
#include "ModuleScene.h"

SpellSpawnManager* SpellSpawnManager::instance = nullptr;

SpellSpawnManager* SpellSpawnManager::GetInstance()
{
    if (instance == nullptr)
    {
        SceneGame* scene = (SceneGame*)Application::GetInstance()->scene->scenes[Application::GetInstance()->scene->currentScene];
        instance = new SpellSpawnManager(scene->player->playerClass);
    }

    return instance;
}

void SpellSpawnManager::SpawnSpell(iPoint p)
{
    if (classTree == nullptr) return;
   
    
    int randomSpell;
    /*if (pClass == PlayerClass::REVENANT) randomSpell = rand() % revenantSpells + 1;
    else randomSpell = (rand() % revenantSpells + 1) + sageSpells;
    */

    if (pClass == PlayerClass::REVENANT)
    {
        randomSpell = rand() % revenantSpells;

        while (classTree->getCurrentLevel(currentRevenantSpells[randomSpell]) <= 0)
        {
            randomSpell = rand() % revenantSpells;
        }

        new SpellObject(p, (SpellID)currentRevenantSpells[randomSpell], CalculateSpellLevel((SpellID)currentRevenantSpells[randomSpell]));
    }
    else 
    {
        randomSpell = rand() % sageSpells;

        while (classTree->getCurrentLevel(currentSageSpells[randomSpell]) <= 0)
        {
            randomSpell = rand() % sageSpells;
        }

        new SpellObject(p, (SpellID)currentSageSpells[randomSpell], CalculateSpellLevel((SpellID)currentSageSpells[randomSpell]));
    }
 
  
    //new SpellObject(pos, (SpellID)randomSpell, CalculateSpellLevel((SpellID)randomSpell));
}

SpellSpawnManager::SpellSpawnManager(PlayerClass playerClass)
{
    pClass = playerClass;
    classTree = ClassTree::GetInstance();
}

SpellSpawnManager::~SpellSpawnManager()
{
    classTree->ReleaseInstance();
}

void SpellSpawnManager::ReleaseInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
        deleted = true;
    }
}

bool SpellSpawnManager::IsDeleted()
{
    return deleted;
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
