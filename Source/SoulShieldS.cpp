#include "SoulShieldS.h"
#include "Player.h"

SoulShieldS::SoulShieldS()
{
	spellStats = spellStats.child("soul_shield");

	for (int i = 0; i < 3; i++)
	{
		std::string lvl = "lvl" + std::to_string(i + 1);

		shield[i] = spellStats.child("shield").attribute(lvl.c_str()).as_int();
		frames[i] = spellStats.child("duration").attribute(lvl.c_str()).as_int();
	}

	uses = spellStats.child("uses").attribute("quantity").as_int();
}

void SoulShieldS::Execute(int level)
{
	if (currentDuration > 0) player->ChangeShield(-shield[currentLevel]);

	currentLevel = level;
	currentDuration = frames[currentLevel];

	player->ChangeShield(shield[currentLevel]);

	isOn = true;
}

void SoulShieldS::Update()
{
	if (!isOn) return;
	currentDuration--;

	if (currentDuration <= 0)
	{
		player->ChangeShield(-shield[currentLevel]);
		isOn = false;
	}
}

void SoulShieldS::CleanUp()
{
}
