#include "Spell.h"

Spell::Spell()
{
}

void Spell::Execute(int level)
{
	printf("Empty Spell Slot!\n");
}

void Spell::Update()
{
}

void Spell::CleanUp()
{
}

int Spell::GetUses()
{
	return uses;
}
