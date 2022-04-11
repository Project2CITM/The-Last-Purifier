#include "Projectile.h"
#include "ModulePhysics.h"

Projectile::Projectile(std::string name, iPoint position, fPoint speed, float damage, bool fire, bool stun, bool isEnemy) : GameObject(name, name)
{
	pBody = app->physics->CreateRectangle(position, 4, 4, this);
	b2Filter filter;
	filter.categoryBits = app->physics->PROJECTILE_LAYER;

	if (isEnemy) filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER;
	else filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->PLAYER_LAYER & ~app->physics->PROJECTILE_LAYER;

	pBody->body->GetFixtureList()[0].SetFilterData(filter);

	this->damage = damage;
	this->isEnemy = isEnemy;
	this->stun = stun;

	if (fire) FireProjectile(speed);

	// Particle Effect
}

void Projectile::FireProjectile(fPoint speed)
{
	b2Vec2 s = { speed.x, speed.y };
	pBody->body->SetLinearVelocity(s);
}

void Projectile::OnCollisionEnter(PhysBody* col)
{
	pendingToDelete = true;
	// Particle effect
}
