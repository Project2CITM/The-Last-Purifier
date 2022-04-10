#include "PlayerStats.h"
#include "CommonTree.h"
#include "ClassTree.h"

PlayerStats::PlayerStats(Player* player)
{
	this->player = player;

	// Create Trees
	commonTree = new CommonTree();
}

void PlayerStats::Start()
{
	commonTree->Start();

	// Load XML with default values
	pugi::xml_parse_result result;
	result = playerValuesXml.load_file("PlayerStats.xml");
	if (result == NULL)
	{
		LOG("Could not load xml file: %s. pugi error: %s", "PlayerStats.xml", result.description());
	}
	else
	{
		defaultValues = playerValuesXml.child("stats").child("common");
	}

	if (player->playerClass == PlayerClass::REVENANT) classValues = playerValuesXml.child("stats").child("revenant");
	else if (player->playerClass == PlayerClass::SAGE) classValues = playerValuesXml.child("stats").child("sage");

	UpdatePlayerStats();
}

void PlayerStats::UpdatePlayerStats()
{
	// Common tree variables
	player->hpPlayer = defaultValues.child("hp").attribute("quantity").as_int() + commonTree->getValue(CommonUpgrades::HEALTH);
	player->dashSpeed = defaultValues.child("speed").attribute("dash").as_float();
	player->movementSpeed = defaultValues.child("speed").attribute("movement").as_float();
	player->spellSlots = defaultValues.child("slots").attribute("spell").as_int() + commonTree->getValue(CommonUpgrades::SKILL_SLOT);
	player->deckSlots = defaultValues.child("slots").attribute("deck").as_int() + commonTree->getValue(CommonUpgrades::DECK);
	player->shield = defaultValues.child("shield").attribute("quantity").as_int();
	player->luck = defaultValues.child("luck").attribute("quantity").as_int() + commonTree->getValue(CommonUpgrades::LUCK);
	player->soulGain = defaultValues.child("souls").attribute("gain").as_int() + commonTree->getValue(CommonUpgrades::SOUL_GAIN);

	player->armour = classValues.child("armour").attribute("quantity").as_int() + commonTree->getValue(CommonUpgrades::ARMOUR);
	player->attackSpeed = classValues.child("attack_speed").attribute("quantity").as_int() + commonTree->getValue(CommonUpgrades::ATTACK_SPEED);
	player->damage = classValues.child("damage").attribute("quantity").as_int() + commonTree->getValue(CommonUpgrades::DAMAGE);
}

void PlayerStats::CleanUp()
{
}


