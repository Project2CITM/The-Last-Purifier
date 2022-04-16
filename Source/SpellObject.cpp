#include "SpellObject.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "SceneGame.h"
#include "Player.h"
#include "PlayerController.h"
#include "PlayerCombat.h"

SpellObject::SpellObject(iPoint pos, SpellID id, int level) : GameObject("SpellObject", "SpellObject")
{
	// Spell Information
	info.id = id;
	info.spellLevel = level;
	info.uses = SpellList::GetInstance()->spells[(int)id]->GetUses();

	// Create PhysBody
	pBody = app->physics->CreateRectangle(pos, 8, 14, this);
	b2Filter filter;
	filter.categoryBits = app->physics->TRIGGER_LAYER;
	filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER;
	pBody->body->GetFixtureList()->SetFilterData(filter);
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
