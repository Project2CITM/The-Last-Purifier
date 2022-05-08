#pragma once
#include <map>

#include "ModuleEvents.h"
#include "External/PugiXml/src/pugixml.hpp"

#define COMMON_TREE_XML "BaseCommonTree.xml"
#define SAVE_COMMON_TREE_XML "SaveCommonTree.xml"

enum class CommonUpgrades
{
    NONE = 0,
    SKILL_SLOT,
    DECK,
    SOUL_GAIN,
    HEALTH,
    ARMOUR,
    LUCK,
    DAMAGE,
    ATTACK_SPEED
};

struct TreeElement
{
    int id = 0;
    CommonUpgrades type = CommonUpgrades::NONE;

    int requiresID1 = 0;
    int requiresID2 = 0;

    bool unlocked = false;

    TreeElement (int id, CommonUpgrades type, int requiresID1, int requiresID2, bool unlocked = false)
    {
        this->id = id;
        this->type = type;
        this->requiresID1 = requiresID1;
        this->requiresID2 = requiresID2;
        this->unlocked = unlocked;
    }
};

class CommonTree : public EventListener
{
public:

    static CommonTree* GetInstance();

    void Start();

    void PreUpdate();

    void Update();

    void PostUpdate();

    void ReleaseInstance();

    virtual void CleanUp();

    void GameEventTriggered(GameEvent id) override;

private:
    //Stores the value of an individual upgrade
    std::map<CommonUpgrades, float> upgradesDic;

    //Stores the global value of all unlocked upgrades
    std::map<CommonUpgrades, float> unlockedDic;

    //Stores the Loaded Tree
    List<TreeElement*>* treeList = new List<TreeElement*>();

    //Loads the value of each individual upgrade
    bool LoadDictionary();

    //Loads the tree with all its upgrades
    bool LoadBaseTree();

    //Saves the tree with all the unlocks
    bool SaveLoadTree(bool load = false);

    //Checks all the unlocked Upgrades and Updates the unlockedDic
    bool CheckUpgrades();

    //Increases the value of an attribute
    void IncreaseValue(CommonUpgrades id);

    //Relates a string to a CommonUpgrade
    CommonUpgrades ResolveType(std::string input);

public:
    //Unlocks an Upgrade from the Loaded tree.
    //True if the upgrade has been unlocked without successfuly
    //False if the upgrade can't be unlocked due to requirements
    bool Upgrade(int id);

    //Returns a pointer to a TreeElement
    TreeElement* getElement(int id);

    //Returns the value of the given CommonUpgrade
    float getValue(CommonUpgrades id);

private:

    static CommonTree* instance;

    CommonTree();
    ~CommonTree();

    pugi::xml_document treeFile;
    pugi::xml_document saveFile;
};

