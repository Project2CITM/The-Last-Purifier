#pragma once
#include "Spell.h"
class PneumaS : public Spell
{
public:
	PneumaS();

	void Execute(int level) override;

private:

	int damage[3];

};

