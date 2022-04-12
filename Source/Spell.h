#ifndef _SPELL_
#define _SPELL_

#include <stdio.h>
#include "External/PugiXml/src/pugixml.hpp"

enum class SpellID
{
	NONE = 0,
	PURIFIED_SWORD,
	SOUL_SHIELD,
	FLAME_CAPE,
	SACRED_SHIELD,
	COVER,
	PURIFICATION_SLASH,
	PURIFICATION_THRUST,
	DASH_SLASH,
	TORMENT,
	CREMATORIUM,
	FOTEIROS,
	TYCHAIA,
	PNEUMA,
	EKRISKI,
	EGEIRO,
	TIMORIA,
	ASTRAPI,
	SOTERIA,
	PSYCHE
};

class Player;

class Spell
{
public:
	Spell();

	virtual void Execute(int level);

	virtual void Update();

	virtual void CleanUp();

	int GetUses();

protected:
	int uses = 1;

	Player* player = nullptr;
	
	pugi::xml_node spellStats;
	pugi::xml_document doc;
};
#endif
