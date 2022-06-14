#include "Enemy.h"
#include "ModulePhysics.h"
#include "DamageArea.h"
#include "SpellSpawnManager.h"
#include "SceneGame.h"
#include "ModuleScene.h"
#include "Souls.h"
#include "OrbHP.h"

Enemy::Enemy(std::string name) :GameObject(name, "Enemy")
{
	spawnManager = SpellSpawnManager::GetInstance();
	SceneGame* scene = (SceneGame*)app->scene->scenes[app->scene->currentScene];
	player = scene->player;
}

Enemy::~Enemy()
{
	
}

void Enemy::Start()
{
}

void Enemy::PreUpdate()
{
	GameObject::PreUpdate();
}

void Enemy::Update()
{
	GameObject::Update();
}

void Enemy::PostUpdate()
{
	GameObject::PostUpdate();
}

void Enemy::CleanUp()
{
}

void Enemy::OnCollisionEnter(PhysBody* col)
{
}

void Enemy::OnCollisionExit(PhysBody* col)
{
}

void Enemy::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->tag == "DamageArea") // If hit by a DamageArea
	{
		DamageArea* takenDamage = (DamageArea*)col->gameObject;

		Hit(takenDamage->GetDamage());
	}
	else if(name == "boss" && col->gameObject->name == "bossProjectile") Hit(100);
}

void Enemy::Hit(int damage)
{
	app->events->TriggerEvent(GameEvent::ENEMY_HIT);

	health -= damage;

	if (health <= 0) Die();
}

void Enemy::Die(bool spawnPower, bool spawnSoul, bool spawnOrb)
{
	if (isDie) return;

	isDie = true;

	DisableCollisions();

	this->enable = false;

 	if (app->scene->sceneGettingDeleted) return;

	if (spawnManager != nullptr)
	{
		if (!spawnManager->IsDeleted() && spawnPower)
		{
			int randNum = rand() % 8;
			if (randNum == 1) spawnManager->SpawnSpell(GetPosition());
		}
	}

	if (player != nullptr && spawnSoul) Souls* soul1 = new Souls(GetPosition(), 3);


	int randHP = rand() % 10;
	if (player != nullptr && randHP == 1 && spawnOrb) new OrbHP(GetPosition());

}

void Enemy::DisableCollisions()
{
}
