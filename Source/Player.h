#ifndef _PLAYER_
#define _PLAYER_

#include "Globals.h"

enum class PlayerClass
{
	REVENANT = 0,
	SAGE
};

class PlayerController;
class PlayerStats;
class Application;

class Player
{
public:

	Player();

	virtual void CleanUp();

	PlayerClass playerClass = PlayerClass::REVENANT;
	PlayerController* controller = nullptr;
	PlayerStats* stats = nullptr;

	// Basic stats
	int souls = 0;
	int hpPlayer = 100;
	int hpMax = 100;
	int spellSlots = 1;
	int deckSlots = 2;
	int shield = 0;
	int armour = 20;
	int damage = 20;
	int attackSpeed = 75;
	float movementSpeed = 9.0f;
	float dashSpeed = 5.0f;
	float soulGain = 1.0f;
	float luck = 1.0f;

	// Extra stats
	int extraDamage = 0;
	int stunTime = 0;
	int pushDistance = 0;

	// Spell stats
	bool purifiedSwordOn = false;
};

#endif