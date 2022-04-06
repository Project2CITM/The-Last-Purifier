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
};


#endif