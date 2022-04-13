#include "Enemy.h"
#include "ModulePhysics.h"
#include "DamageArea.h"
#include "SpellSpawnManager.h"

Enemy::Enemy() :GameObject("Enemy", "None")
{
	spawnManager = SpellSpawnManager::GetInstance();
}

Enemy::~Enemy()
{
}

void Enemy::Start()
{
}

void Enemy::PreUpdate()
{
}

void Enemy::Update()
{
}

void Enemy::PostUpdate()
{
}

void Enemy::CleanUp()
{
	if (!spawnManager->IsDeleted()) spawnManager->SpawnSpell(GetPosition());
}

void Enemy::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject == nullptr) return;
	if (col->gameObject->tag == "DamageArea") // If hit by a DamageArea
	{
		DamageArea* takenDamage = (DamageArea*)col->gameObject;

		Hit(*takenDamage->damage);
	}
}

void Enemy::OnCollisionExit(PhysBody* col)
{
}

void Enemy::Hit(int damage)
{
	health -= damage;
	if (health <= 0) Die();
}

void Enemy::Die()
{
	pendingToDelete = true;
}
