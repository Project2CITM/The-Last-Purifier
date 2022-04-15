#include "Player.h"
#include "PlayerController.h"
#include "Application.h"
#include "PlayerStats.h"

Player::Player()
{
	controller = new PlayerController("Player", "Player", this);
}

void Player::CleanUp()
{
	// THE CONTROLLER IS CLEANED UP BY THE SCENE, AS IT IS A GAMEOBJECT AND, THEREFOR, MANAGED BY THE SCENE
	if (stats != nullptr)
	{
		stats->CleanUp();
		RELEASE(stats);
	}
}
