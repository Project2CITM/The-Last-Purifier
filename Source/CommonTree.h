#pragma once
#include <map>

#include "GameObject.h"

enum class CommonUpgrades
{
    NONE = 0,
    DECK,
    SOUL_GAIN,
    HEALTH,
    ARMOUR,
    LUCK,
    DAMAGE,
    ATTACK_SPEED
};

struct CommonTreeElement
{
    int id = 0;
    CommonUpgrades type = CommonUpgrades::NONE;

    int requiresID1 = 0;
    int requiresID2 = 0;
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
    std::map<CommonUpgrades, float> upgradesDic;

    void LoadUpgradesDictionary();
};

