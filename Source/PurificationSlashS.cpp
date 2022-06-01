#include "PurificationSlashS.h"
#include "ModulePhysics.h"
#include "DamageArea.h"
#include "Player.h"
#include "PlayerController.h"
#include "ModulePhysics.h"
#include "ParticlePurificationSlash.h"


PurificationSlashS::PurificationSlashS()
{
	spellStats = spellStats.child("purification_slash");

	for (int i = 0; i < 3; i++)
	{
		std::string lvl = "lvl" + std::to_string(i + 1);

		damage[i] = spellStats.child("damage").attribute(lvl.c_str()).as_int();
	}

	uses = spellStats.child("uses").attribute("quantity").as_int();

	damageArea = new DamageArea({ 0,0 }, 25, 60, 0);
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
	if (offset.x == 0.35f) attackRotation = 90 * DEGTORAD;

	damageArea->pBody->body->SetActive(true);

	damageArea->pBody->body->SetTransform(player->controller->pBody->body->GetPosition() + offset, attackRotation);

	currentFrame = attackFrames;

	iPoint particleOffset;
	int particleRotation = 0;
	switch (player->controller->lookingDir)
	{
	case LookingDirection::DOWN:
		particleOffset = { 53, -36 };
		particleRotation = 90;
		break;
	case LookingDirection::UP:
		particleOffset = { -58, 45 };
		particleRotation = 270;
		break;
	case LookingDirection::LEFT:
		particleOffset = { -32, -64 };
		particleRotation = 180;
		break;
	case LookingDirection::RIGHT:
		particleOffset = { -32, -64 };
		particleRotation = 0;
		break;

	}

	new ParticlePurificationSlash(damageArea->GetPosition() + particleOffset, particleRotation, 0.15f, 0, player->purifiedSwordOn);

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
		offset = { 2.10f, -0.45f };
		break;
	case LookingDirection::LEFT:
		offset = { -1.30f, -0.45f };
		break;
	case LookingDirection::UP:
		offset = { 0.35f, -2.75f };
		break;
	case LookingDirection::DOWN:
		offset = { 0.35f, 1.55f };
		break;
	}

	return offset;
}
