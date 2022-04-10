#include "PlayerSage.h"
#include "PlayerStats.h"

PlayerSage::PlayerSage() : Player()
{
	playerClass = PlayerClass::SAGE;
	stats = new PlayerStats(this);
	// In the future here will be initialized every animation and spirte of this class.
}
