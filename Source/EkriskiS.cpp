#include "EkriskiS.h"
#include "EkriskiDamageArea.h"
#include "Player.h"
#include "PlayerController.h"
#include "PlayerCombat.h"

EkriskiS::EkriskiS()
{
	spellStats = spellStats.child("ekriski");

	for (int i = 0; i < 3; i++)
	{
		std::string lvl = "lvl" + std::to_string(i + 1);

		damage[i] = spellStats.child("damage").attribute(lvl.c_str()).as_int();
	}

	uses = spellStats.child("uses").attribute("quantity").as_int();
}

void EkriskiS::Execute(int level)
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
	new EkriskiDamageArea(pos, 40, 40, damage[level-1]);
}

void EkriskiS::Update()
{
}

void EkriskiS::CleanUp()
{
}
