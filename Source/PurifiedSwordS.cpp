#include "PurifiedSwordS.h"
#include <string>

PurifiedSwordS::PurifiedSwordS() : Spell(), EventListener(GameEvent::PLAYER_ATTACK)
{
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
}

PurifiedSwordS::~PurifiedSwordS()
{

}

void PurifiedSwordS::Execute(int level)
{
	printf("Execute Purified Sword at level %d\n", level);
}

void PurifiedSwordS::Update()
{
}

void PurifiedSwordS::CleanUp()
{
	app->events->RemoveListener(this);
}

void PurifiedSwordS::GameEventTriggered()
{
	printf("\nPurified Sword detected Player Attack\n");
}
