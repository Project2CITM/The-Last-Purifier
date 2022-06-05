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
	spellName = spellStats.child("name").attribute("name").as_string();
}

void PneumaS::Execute(int level)
{
	iPoint pos = player->controller->GetPosition();

	int width = 60;
	int height = 15;
	int temp = 0;
	switch (player->controller->lookingDir)
	{
	case LookingDirection::RIGHT:
		pos.x += 60;
		pos.y -= 10;
		break;
	case LookingDirection::LEFT:
		pos.x -= 40;
		pos.y -= 10;
		break;
	case LookingDirection::UP:
		pos.x += 10;
		pos.y -= 70;
		temp = width;
		width = height;
		height = temp;
		break;
	case LookingDirection::DOWN:
		pos.x += 10;
		pos.y += 50;
		temp = width;
		width = height;
		height = temp;
		break;
	default:
		break;
	}
	new PneumaDamageArea(pos, width, height, damage[level - 1]);
}
