#pragma once
#include "Spell.h"
#include "Globals.h"

class SoulShieldS : public Spell
{
public:
	SoulShieldS();

	void Execute(int level) override;

	void Update() override;

	void CleanUp() override;

private:
	int shield[3];
	int frames[3];

	int currentDuration = 0;
	int currentLevel;

	bool isOn = false;

	uint executeSFX;

};

