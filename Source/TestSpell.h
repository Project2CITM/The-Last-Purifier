#pragma once
#include "Spell.h"
class TestSpell : public Spell
{
public:
	TestSpell();

	bool Execute() override;

	void Update() override;

};

