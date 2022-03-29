#pragma once
#include "Spell.h"
class TestSpell : public Spell
{
public:
	TestSpell();

	void Execute(int level) override;
	
	void Update() override;

};

