#include "TestSpell.h"
#include <stdio.h>

TestSpell::TestSpell() : Spell()
{
	this->uses = spellStats.child("purified_sword").child("uses").attribute("quantity").as_int();
}

void TestSpell::Execute(int level)
{
	printf("Execute Test Spell at level %d!\n", level);
}

void TestSpell::Update()
{
}
