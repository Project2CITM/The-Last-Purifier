#include "PlayerSage.h"
#include "PlayerStats.h"
#include "ModuleTextures.h"
#include "Animation.h"
#include "PlayerController.h"

PlayerSage::PlayerSage() : Player()
{
	playerClass = PlayerClass::SAGE;
	stats = new PlayerStats(this);
	InitClassSprites(playerClass);
}