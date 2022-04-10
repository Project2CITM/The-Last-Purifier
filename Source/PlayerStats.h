#ifndef _PLAYER_STATS_
#define _PLAYER_STATS_

#include "Player.h"
#include "External/PugiXml/src/pugixml.hpp"

class CommonTree;
class ClassTree;

class PlayerStats
{
public:
	PlayerStats(Player* player);

	void Start();

	void UpdatePlayerStats();

	void CleanUp();

	int currentSouls = 0;
private:
	Player* player;
	CommonTree* commonTree;

	pugi::xml_document playerValuesXml;
	pugi::xml_node defaultValues;
	pugi::xml_node classValues;
};
#endif

