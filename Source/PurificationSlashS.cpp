#include "PurificationSlashS.h"
#include "ModulePhysics.h"
#include "DamageArea.h"
#include "Player.h"
#include "PlayerController.h"
#include "ModulePhysics.h"


PurificationSlashS::PurificationSlashS()
{
	spellStats = spellStats.child("purification_slash");

	for (int i = 0; i < 3; i++)
	{
		std::string lvl = "lvl" + std::to_string(i + 1);

		damage[i] = spellStats.child("damage").attribute(lvl.c_str()).as_int();
	}

	uses = spellStats.child("uses").attribute("quantity").as_int();

	damageArea = new DamageArea({ 0,0 }, 25, 45, 0);
	damageArea->pBody->body->SetActive(false);

	b2Filter filter;
	filter.categoryBits = Application::GetInstance()->physics->PLAYER_LAYER;

	damageArea->pBody->body->GetFixtureList()->SetFilterData(filter);
}

void PurificationSlashS::Execute(int level)
{
	damageArea->damage = damage[level-1];

	b2Vec2 offset = GetPlayerOffset();
	float attackRotation = 0;
	if (offset.x == 0.25f) attackRotation = 90 * DEGTORAD;

	damageArea->pBody->body->SetActive(true);

	damageArea->pBody->body->SetTransform(player->controller->pBody->body->GetPosition() + offset, attackRotation);

	currentFrame = attackFrames;

}

void PurificationSlashS::Update()
{
	if (currentFrame < 0) return;
	currentFrame--;
	if (currentFrame <= 0) damageArea->pBody->body->SetActive(false);
}

void PurificationSlashS::CleanUp()
{
}

b2Vec2 PurificationSlashS::GetPlayerOffset()
{
	b2Vec2 offset;

	switch (player->controller->lookingDir)
	{
	case LookingDirection::RIGHT:
		offset = { 1.5f, -0.75f };
		break;
	case LookingDirection::LEFT:
		offset = { -0.75f, -0.75f };
		break;
	case LookingDirection::UP:
		offset = { 0.25f, -2.0f };
		break;
	case LookingDirection::DOWN:
		offset = { 0.25f, 0.75f };
		break;
	}

	return offset;
}
