#ifndef _PLAYER_
#define _PLAYER_

#include "Globals.h"

enum class PlayerClass
{
	REVENANT = 0,
	SAGE
};

class PlayerController;
class Application;

class Player
{
public:
	Player();

	virtual void CleanUp();

	PlayerClass playerClass = PlayerClass::REVENANT;
	PlayerController* controller = nullptr;

	int hpPlayer = 100;
	int spellSlots = 1;
	int deckSlots = 2;
	int shield = 0;
	int armour = 20;
	int damage = 20;
	int attackSpeed = 75;
	int movementSpeed = 9.0f;
	int dashSpeed = 5;
	float soulGain = 1.0f;
	float luck = 1.0f;

};


#endif