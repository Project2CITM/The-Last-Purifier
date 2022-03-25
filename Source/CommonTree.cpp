#include "CommonTree.h"

CommonTree::CommonTree() : GameObject(name, tag)
{

}

CommonTree::~CommonTree()
{

}

void CommonTree::Start()
{

}

void CommonTree::PreUpdate()
{

}

void CommonTree::Update()
{

}

void CommonTree::PostUpdate()
{

}

void CommonTree::CleanUp()
{

}

void CommonTree::LoadUpgradesDictionary()
{
	/*
		THIS WILL BE A LOAD FILE FUNCTION IN THE FUTURE
	*/


	upgradesDic.insert({ CommonUpgrades::DECK, 1.0f });
	upgradesDic.insert({ CommonUpgrades::SOUL_GAIN, 1.0f });
	upgradesDic.insert({ CommonUpgrades::HEALTH, 5.0f });
	upgradesDic.insert({ CommonUpgrades::ARMOUR, 4.0f });
	upgradesDic.insert({ CommonUpgrades::LUCK, 1.0f });
	upgradesDic.insert({ CommonUpgrades::DAMAGE, 4.0f });
	upgradesDic.insert({ CommonUpgrades::ATTACK_SPEED, 0.05f });
}