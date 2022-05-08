#include "WeaponObject.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "Player.h"
#include "PlayerController.h"
#include "PlayerCombat.h"
#include "ModuleScene.h"
#include "SceneGame.h"

WeaponObject::WeaponObject(iPoint pos) : GameObject("WeaponObject", "WeaponObject")
{
	SceneGame* scene = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	this->playerCombat = scene->player->controller->combat;

	pBody = app->physics->CreateCircle(pos.x, pos.y, 10, this, true);
}

void WeaponObject::PreUpdate()
{
	if (!isPlayerIn) return;

	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN || app->input->GetControllerButton(BUTTON_A) == KEY_DOWN)
	{
		if (weaponInfo.weaponClass == PlayerClass::REVENANT) playerCombat->ChangeRevenantWeapon(weaponInfo.revenantWeaponID);
		else playerCombat->ChangeSageWeapon(weaponInfo.sageWeaponID);
	}
}

void WeaponObject::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->name == "Player")
	{
		isPlayerIn = true;
	}
}
void WeaponObject::OnCollisionExit(PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->name == "Player")
	{
		isPlayerIn = false;
	}
}
