#include "Player.h"
#include "PlayerController.h"
#include "Application.h"

Player::Player()
{
	controller = new PlayerController("player", "Player", this);
}

void Player::CleanUp()
{
	// THE CONTROLLER IS CLEANED UP BY THE SCENE, AS IT IS A GAMEOBJECT AND, THEREFOR, MANAGED BY THE SCENE
}
