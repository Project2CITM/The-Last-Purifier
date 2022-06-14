#include "FoteirosS.h"
#include "Projectile.h"
#include "PlayerCombat.h"
#include "PlayerController.h"
#include "Player.h"
#include "ModulePhysics.h"

FoteirosS::FoteirosS()
{
	spellStats = spellStats.child("foteiros");

	for (int i = 0; i < 3; i++)
	{
		std::string lvl = "lvl" + std::to_string(i + 1);

		damage[i] = spellStats.child("damage").attribute(lvl.c_str()).as_int();
	}

	uses = spellStats.child("uses").attribute("quantity").as_int();
	spellName = spellStats.child("name").attribute("name").as_string();
}

void FoteirosS::Execute(int level)
{
	iPoint offset = { METERS_TO_PIXELS(player->controller->combat->GetAttackOffset().x), METERS_TO_PIXELS(player->controller->combat->GetAttackOffset().y) };
	fPoint direction = player->controller->combat->GetProjectileOffset();
	int rot = player->controller->combat->GetProjectileRotation();

	new Projectile(player->controller->GetPosition() + offset, damage[level - 1], rot, direction * 20);
}

void FoteirosS::CleanUp()
{
}