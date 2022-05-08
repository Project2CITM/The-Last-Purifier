#include "SageWeapon.h"

SageWeapon::SageWeapon(PlayerController* playerController)
{
	this->playerController = playerController;
	app = Application::GetInstance();
}

bool SageWeapon::Attack(int chargedTime)
{
	return true;
}

void SageWeapon::PreUpdate()
{
}

void SageWeapon::UpdateAttackBar(bool holding, int deltaTimeMS)
{
}

void SageWeapon::Update()
{
}

void SageWeapon::PostUpdate()
{
}

void SageWeapon::CleanUp()
{
}
