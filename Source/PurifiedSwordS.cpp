#include "PurifiedSwordS.h"
#include <string>
#include "Player.h"


PurifiedSwordS::PurifiedSwordS() : Spell()
{
	this->listenTo[0] = GameEvent::PLAYER_ATTACK;

	Application::GetInstance()->events->AddListener(this);

	spellStats = spellStats.child("purified_sword");

	for (int i = 0; i < 3; i++)
	{
		std::string lvl = "lvl" + std::to_string(i + 1);

		damage[i] = spellStats.child("damage").attribute(lvl.c_str()).as_int();
		stun[i] = spellStats.child("stun").attribute(lvl.c_str()).as_bool();
		stunDuration[i] = spellStats.child("stun_duration").attribute(lvl.c_str()).as_int();
	}

	uses = spellStats.child("uses").attribute("quantity").as_int();

	spellName = spellStats.child("name").attribute("name").as_string();
}

PurifiedSwordS::~PurifiedSwordS()
{

}

void PurifiedSwordS::Execute(int level)
{
	// Si ya habiamos usado el Spell y lo volvemos a usar, quitamos el daño extra aplicado anteriormente
	if (currentAttacks != 0)
	{
		player->extraDamage -= damage[currentLevel];
	}
	currentLevel = level-1;
	player->extraDamage += damage[currentLevel];
	currentAttacks = attackUses;
	player->purifiedSwordOn = true;
}

void PurifiedSwordS::Update()
{
}

void PurifiedSwordS::CleanUp()
{
	Application::GetInstance()->events->RemoveListener(this);
}

void PurifiedSwordS::GameEventTriggered(GameEvent id)
{
	if (currentAttacks <= 0) return;

	currentAttacks--;

	if (currentAttacks <= 0)
	{
		player->extraDamage -= damage[currentLevel];
		player->purifiedSwordOn = false;
	}

	printf("\nPurified Sword detected Player Attack\n");
}
