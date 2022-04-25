#ifndef _PLAYER_STATS_
#define _PLAYER_STATS_

#include "Player.h"
#include "External/PugiXml/src/pugixml.hpp"
#include "ModuleEvents.h"

class CommonTree;
class ClassTree;

class PlayerStats : public EventListener
{
public:
	PlayerStats(Player* player);

	void Start();

	void UpdatePlayerStats();

	void SavePlayerProgress();

	void GameEventTriggered(GameEvent id) override;

	void CleanUp();

	int currentSouls = 0;
	CommonTree* commonTree = nullptr;
private:
	Player* player = nullptr;


	pugi::xml_document playerValuesXml;
	pugi::xml_node defaultValues;
	pugi::xml_node classValues;
};
#endif

