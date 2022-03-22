#include "Player.h"
#include "PlayerController.h"
#include "Application.h"

Player::Player(Application* app)
{
	controller = new PlayerController("player", "Player", app, this);
}

void Player::CleanUp()
{
	// THE CONTROLLER IS CLEANED UP BY THE SCENE, AS IT IS A GAMEOBJECT AND, THEREFOR, MANAGED BY THE SCENE
}
