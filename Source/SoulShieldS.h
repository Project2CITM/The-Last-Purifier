#pragma once
#include "Spell.h"
class SoulShieldS : public Spell
{
public:
	SoulShieldS();

	void Execute(int level) override;

	void Update() override;

	void CleanUp() override;

};

