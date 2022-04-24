#include "PlayerShadow.h"
#include "ModuleTextures.h"

PlayerShadow::PlayerShadow(GameObject* follow) : GameObject("PlayerShadow", "PlayerShadow")
{
	this->follow = follow;
	renderObjects[0].InitAsTexture(app->textures->Load("Assets/Sprites/Player/PlayerShadow.png"), follow->GetPosition() + offset, { 0,0,0,0 }, 1, 1, -0.5f);
	renderObjects[0].orderOnHeight = false;
}

void PlayerShadow::PreUpdate()
{
	if (follow->pendingToDelete)
	{
		pendingToDelete = true;
		return;
	}
	if (pendingToDelete) return;
	SetPosition(follow->GetPosition() + offset);
}

void PlayerShadow::PostUpdate()
{
	GameObject::PostUpdate();
}
