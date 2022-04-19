#pragma once
#include "Spell.h"
class FoteirosS : public Spell
{
public:

	FoteirosS();

	void Execute(int level) override;

	void Update() override;

	void CleanUp() override;

private:
	int damage[3];
};

