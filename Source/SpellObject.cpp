#include "SpellObject.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "SceneGame.h"
#include "Player.h"
#include "PlayerController.h"
#include "PlayerCombat.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"

SpellObject::SpellObject(iPoint pos, SpellID id, int level) : GameObject("SpellObject", "SpellObject")
{
	// Spell Information
	info.id = id;
	info.spellLevel = level;
	info.uses = SpellList::GetInstance()->spells[(int)id]->GetUses();

	// Create PhysBody
	pBody = app->physics->CreateRectangleSensor(pos, 8, 8, this);
	b2Filter filter;
	filter.categoryBits = app->physics->TRIGGER_LAYER;
	filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER;
	pBody->body->GetFixtureList()->SetFilterData(filter);
	// Add Texture
	renderObjects[0].InitAsTexture(app->textures->Load("Assets/Sprites/UI/iconsSpells.png"), pos, { 0,0,0,0 },0.5f);
	renderObjects[0].section = { 0,31 * (int)id, 31,31 };

	pickupspellFX = app->audio->LoadFx("Audio/SFX/Player/sfx_playerPickUpSpell.wav");

	SetColorFromLevel();
}

void SpellObject::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->name == "Player")
	{
		if (pendingToDelete) return;
		
		SceneGame* scene = (SceneGame*)app->scene->scenes[app->scene->currentScene];

		if (scene->player->controller->combat->AddSpell(info))
		{
			pendingToDelete = true;
			app->audio->PlayFx(pickupspellFX);
		}
	}
	
}

void SpellObject::SetColorFromLevel()
{
	switch (this->info.spellLevel)
	{
	case 2:
		renderObjects[0].SetColor({ 0,155,0,255 });
		break;
	case 3:
		renderObjects[0].SetColor({ 75,75,0,255 });
		break;
	}
}

void SpellObject::PostUpdate()
{
	renderObjects[0].destRect.x = GetDrawPosition().x - 7;
	renderObjects[0].destRect.y = GetDrawPosition().y - 8;

	app->renderer->AddRenderObjectRenderQueue(renderObjects[0]);
}
