#include "RevenantWeapon.h"
#include "PlayerController.h"

RevenantWeapon::RevenantWeapon(PlayerController* playerController)
{
	this->playerController = playerController;
	app = Application::GetInstance();
}

bool RevenantWeapon::Attack(int chargedTime)
{
	return true;
}

void RevenantWeapon::PreUpdate()
{
}

void RevenantWeapon::Update()
{
}

void RevenantWeapon::PostUpdate()
{
}

void RevenantWeapon::CleanUp()
{
}
