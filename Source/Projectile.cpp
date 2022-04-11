#include "Projectile.h"
#include "ModulePhysics.h"
#include "DamageArea.h"

Projectile::Projectile(std::string name, iPoint position, fPoint speed, int damage, bool fire, bool stun, bool isEnemy) : GameObject(name, name)
{
	this->damage = damage;
	this->isEnemy = isEnemy;
	this->stun = stun;

	damageArea = new DamageArea(position, 4, 4, &this->damage, &this->stun);
	pBody = app->physics->CreateRectangle(position, 4, 4, this);
	b2Filter filter;
	filter.categoryBits = app->physics->PROJECTILE_LAYER;

	if (isEnemy) filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER;
	else filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->PLAYER_LAYER & ~app->physics->PROJECTILE_LAYER;

	pBody->body->GetFixtureList()[0].SetFilterData(filter);
	damageArea->pBody->body->GetFixtureList()[0].SetFilterData(filter);

	if (fire) FireProjectile(speed);

	// Particle Effect
}

void Projectile::FireProjectile(fPoint speed)
{
	b2Vec2 s = { speed.x, speed.y };
	damageArea->pBody->body->SetLinearVelocity(s);
	pBody->body->SetLinearVelocity(s);
}

void Projectile::OnCollisionEnter(PhysBody* col)
{
	pendingToDelete = true;
	damageArea->pendingToDelete = true;
	// Particle effect
}
