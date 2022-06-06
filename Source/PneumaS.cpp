#include "PneumaS.h"
#include "Player.h"
#include "PlayerController.h"
#include "PlayerCombat.h"
#include "PneumaDamageArea.h"
#include "ParticlePneuma.h"

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
	iPoint damageAreaPos = player->controller->GetPosition();
	iPoint particlePos = player->controller->GetPosition();
	int width = 30;
	int height = 15;
	int temp = 0;

	int rotation = 0;
	switch (player->controller->lookingDir)
	{
	case LookingDirection::RIGHT:
		damageAreaPos.x += 60;
		damageAreaPos.y -= 10;

		particlePos.x += 105;
		particlePos.y -= 60;

		rotation = 90;
		break;
	case LookingDirection::LEFT:
		damageAreaPos.x -= 40;
		damageAreaPos.y -= 10;

		particlePos.x -= 80;
		particlePos.y += 45;

		rotation = 270;
		break;
	case LookingDirection::UP:
		damageAreaPos.x += 10;
		damageAreaPos.y -= 70;

		particlePos.x -= 45;
		particlePos.y -= 120;

		temp = width;
		width = height;
		height = temp;
		break;
	case LookingDirection::DOWN:
		damageAreaPos.x += 10;
		damageAreaPos.y += 50;

		particlePos.x += 60;
		particlePos.y += 90;

		temp = width;
		width = height;
		height = temp;
		rotation = 180;
		break;
	default:
		break;
	}
	new PneumaDamageArea(damageAreaPos, width, height, damage[level - 1]);
	new ParticlePneuma(particlePos, rotation, 240);
}
