#include "PlayerStats.h"
#include "CommonTree.h"
#include "ClassTree.h"


PlayerStats::PlayerStats(Player* player)
{
	this->listenTo[0] = GameEvent::SAVE_GAME;
	this->listenTo[1] = GameEvent::UPDATE_COMMON_TREE;

	this->player = player;

	Application::GetInstance()->events->AddListener(this);

	// Create Trees
	commonTree = CommonTree::GetInstance();
}

void PlayerStats::Start()
{
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

void PlayerStats::UpdatePlayerStats(bool souls_update)
{
	// Common tree variables
	if(souls_update) player->souls = defaultValues.child("souls").attribute("quantity").as_int();

	player->hpPlayer = defaultValues.child("hp").attribute("quantity").as_int() + commonTree->getValue(CommonUpgrades::HEALTH);
	player->dashSpeed = defaultValues.child("speed").attribute("dash").as_float();
	player->movementSpeed = defaultValues.child("speed").attribute("movement").as_float();
	player->spellSlots = defaultValues.child("slots").attribute("spell").as_int() + commonTree->getValue(CommonUpgrades::SKILL_SLOT);
	player->deckSlots = defaultValues.child("slots").attribute("deck").as_int() + commonTree->getValue(CommonUpgrades::DECK);
	player->shield = defaultValues.child("shield").attribute("quantity").as_int();
	player->luck = defaultValues.child("luck").attribute("quantity").as_int() + commonTree->getValue(CommonUpgrades::LUCK);
	player->soulGain = defaultValues.child("souls").attribute("gain").as_int() + commonTree->getValue(CommonUpgrades::SOUL_GAIN);
	player->armour = classValues.child("armour").attribute("quantity").as_int() + commonTree->getValue(CommonUpgrades::ARMOUR);
	player->attackSpeed = classValues.child("attack_speed").attribute("quantity").as_int() - commonTree->getValue(CommonUpgrades::ATTACK_SPEED);
	player->damage = classValues.child("damage").attribute("quantity").as_int() + commonTree->getValue(CommonUpgrades::DAMAGE);

	player->hpMax = player->hpPlayer;
}

void PlayerStats::SavePlayerProgress()
{
	pugi::xml_node n = playerValuesXml.child("stats");

	n.child("currentClass").attribute("class") = (int)player->playerClass;

	n.child("common").child("souls").attribute("quantity") = player->souls;

	playerValuesXml.save_file("PlayerStats.xml");
}

void PlayerStats::GameEventTriggered(GameEvent id)
{
	switch (id)
	{
	case GameEvent::SAVE_GAME:
		SavePlayerProgress();
		break;
	case GameEvent::UPDATE_COMMON_TREE:
		UpdatePlayerStats(false);
		Application::GetInstance()->events->TriggerEvent(GameEvent::UPDATE_PLAYER_STATS);
		break;
	}
	SavePlayerProgress();
	
}

void PlayerStats::CleanUp()
{
	Application::GetInstance()->events->RemoveListener(this);

	commonTree->ReleaseInstance();
}
