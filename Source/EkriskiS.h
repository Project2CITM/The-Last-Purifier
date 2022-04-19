#pragma once
#include "Spell.h"
class EkriskiS : public Spell
{
public:
	EkriskiS();

	void Execute(int level) override;

	void Update() override;

	void CleanUp() override;

private:

	int damage[3];
};

