#include "WeaponObject.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "Player.h"
#include "PlayerController.h"
#include "PlayerCombat.h"
#include "ModuleScene.h"
#include "SceneGame.h"
#include "ModuleTextures.h"

WeaponObject::WeaponObject(iPoint pos, WeaponInfo info) : GameObject("WeaponObject", "WeaponObject")
{
	this->weaponInfo = info;

	SetRenderObjectFromWeaponInfo();

	SceneGame* scene = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	this->playerCombat = scene->player->controller->combat;

	pBody = app->physics->CreateCircle(pos.x, pos.y, 50, this, true);
	b2Filter filter;
	filter.categoryBits = app->physics->TRIGGER_LAYER;
	filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER;
	pBody->body->GetFixtureList()->SetFilterData(filter);

	weaponText = new Text(this->position, "Press F to collect weapon.");

}

WeaponObject::~WeaponObject()
{

}

void WeaponObject::PreUpdate()
{
	if (!isPlayerIn) return;

	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN || app->input->GetControllerButton(BUTTON_A) == KEY_DOWN)
	{
		if (weaponInfo.weaponClass == PlayerClass::REVENANT) playerCombat->ChangeRevenantWeapon(weaponInfo.revenantWeaponID);
		else playerCombat->ChangeSageWeapon(weaponInfo.sageWeaponID);

		this->pendingToDelete = true;
		this->weaponText->pendingToDelate = true;
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

void WeaponObject::SetRenderObjectFromWeaponInfo()
{
	if (weaponInfo.weaponClass == PlayerClass::REVENANT)
	{
		renderObjects[0].InitAsTexture(app->textures->Load("Sprites/Armas/revenantWeapons76x300.png", false, false), position, { 0,0,0,0 }, 0.25f, 2, 1.0f);
		switch (weaponInfo.revenantWeaponID)
		{
		case RevenantWeaponIDs::SWORD:
			renderObjects[0].section = { 0,0,76,300 };
			break;
		case RevenantWeaponIDs::SPEAR:
			renderObjects[0].section = { 76,0,76,300 };
			break;
		}
		renderObjects[0].textureCenterX = 38;
		renderObjects[0].textureCenterY = 150;
	}
	else
	{
		renderObjects[0].InitAsTexture(app->textures->Load("Sprites/Armas/sageWeapons.png", false, false), position, { 0,0,0,0 }, 0.1f, 2, 1.0f);
		switch (weaponInfo.sageWeaponID)
		{
		case SageWeaponIDs::STAFF:
			renderObjects[0].section = { 0,0,562,429 };
			renderObjects[0].textureCenterX = 281;
			renderObjects[0].textureCenterY = 214;
			break;
		case SageWeaponIDs::BOOK:
			renderObjects[0].section = { 562,0,499,429 };
			renderObjects[0].textureCenterX = 250;
			renderObjects[0].textureCenterY = 214;
			break;
		}
	}
}
