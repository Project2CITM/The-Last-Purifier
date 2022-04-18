#ifndef _SPELL_
#define _SPELL_

#include <stdio.h>
#include "External/PugiXml/src/pugixml.hpp"

enum class SpellID
{
	NONE = 0,
	PURIFIED_SWORD,//1
	SOUL_SHIELD,//2
	FLAME_CAPE,
	SACRED_SHIELD,
	COVER,
	PURIFICATION_SLASH,//6
	PURIFICATION_THRUST,
	DASH_SLASH,
	TORMENT,
	CREMATORIUM,
	FOTEIROS,//11
	TYCHAIA,
	PNEUMA,
	EKRISKI,//14
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

	~Spell();

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
