#include "TestSpell.h"
#include <stdio.h>

TestSpell::TestSpell() : Spell()
{
	this->listenTo[0] = GameEvent::PLAYER_ATTACK;

	this->uses = spellStats.child("purified_sword").child("uses").attribute("quantity").as_int();
	//Application::GetInstance()->events->AddListener(this);
}

void TestSpell::Execute(int level)
{
	printf("Execute Test Spell at level %d!\n", level);
}

void TestSpell::Update()
{
}

void TestSpell::GameEventTriggered(GameEvent id)
{
	printf("Detected Event Trigger!!");
}
