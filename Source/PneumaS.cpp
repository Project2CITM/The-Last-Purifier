#include "PneumaS.h"
#include "Player.h"
#include "PlayerController.h"
#include "PlayerCombat.h"
#include "PneumaDamageArea.h"

PneumaS::PneumaS()
{
	spellStats = spellStats.child("pneuma");

	for (int i = 0; i < 3; i++)
	{
		std::string lvl = "lvl" + std::to_string(i + 1);

		damage[i] = spellStats.child("damage").attribute(lvl.c_str()).as_int();
	}

	uses = spellStats.child("uses").attribute("quantity").as_int();

}

void PneumaS::Execute(int level)
{
	iPoint pos = player->controller->GetPosition();

	switch (player->controller->lookingDir)
	{
	case LookingDirection::RIGHT:
		pos.x += 60;
		break;
	case LookingDirection::LEFT:
		pos.x -= 40;
		break;
	case LookingDirection::UP:
		pos.x += 10;
		pos.y -= 70;
		break;
	case LookingDirection::DOWN:
		pos.x += 10;
		pos.y += 50;
		break;
	default:
		break;
	}
	new PneumaDamageArea(pos, damage[level - 1]);
}
