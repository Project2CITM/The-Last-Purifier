#include "PlayerRevenant.h"
#include "Application.h"
#include "PlayerController.h"
#include "PlayerStats.h"

PlayerRevenant::PlayerRevenant() : Player()
{
	playerClass = PlayerClass::REVENANT;
	stats = new PlayerStats(this);
	// In the future here will be initialized every animation and spirte of this class.
}



