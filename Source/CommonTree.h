#pragma once
#include <map>

#include "GameObject.h"
#include "External/PugiXml/src/pugixml.hpp"

#define COMMON_TREE_XML "CommonTree.xml"

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

class CommonTree :
    public GameObject
{
public:
    CommonTree();
    ~CommonTree();

    void Start() override;

    void PreUpdate() override;

    void Update() override;

    void PostUpdate() override;

    virtual void CleanUp();

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
    bool LoadTree();

    //Saves the tree with all the unlocks
    bool SaveTree();

    //Checks all the unlocked Upgrades and Updates the unlockedDic
    bool CheckUpgrades();

    //Increases the value of an attribute
    void IncreaseValue(CommonUpgrades id);
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
    pugi::xml_document treeFile;
};

