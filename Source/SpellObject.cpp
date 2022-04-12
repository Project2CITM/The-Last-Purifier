#include "SpellObject.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "SceneGame.h"
#include "Player.h"
#include "PlayerController.h"
#include "PlayerCombat.h"

SpellObject::SpellObject(iPoint position, SpellID id, int level) : GameObject("SpellObject", "SpellObject")
{
	// Spell Information
	info.id = id;
	info.spellLevel = level;
	info.uses = SpellList::GetInstance()->spells[(int)id]->GetUses();

	// Create PhysBody
	pBody = app->physics->CreateRectangle(position, 8, 14, this);

	// Add Texture
}

void SpellObject::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->name == "Player")
	{
		if (pendingToDelete) return;
		
		SceneGame* scene = (SceneGame*)app->scene->scenes[app->scene->currentScene];

		scene->player->controller->combat->AddSpell(info);

		pendingToDelete = true;
	}
	
}
