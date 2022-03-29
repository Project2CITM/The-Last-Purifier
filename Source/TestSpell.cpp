#include "TestSpell.h"
#include <stdio.h>

TestSpell::TestSpell()
{
	this->uses = 2;
}

void TestSpell::Execute(int level)
{
	printf("Execute Test Spell at level %d!\n", level);
}

void TestSpell::Update()
{
}
