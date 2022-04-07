#include "Projectile.h"
#include "ModulePhysics.h"

Projectile::Projectile(std::string name, iPoint position, fPoint speed, float damage, bool fire, bool stun, bool isEnemy) : GameObject(name, name)
{
	pBody = app->physics->CreateRectangle(position, 4, 4, this);

	this->damage = damage;
	this->isEnemy = isEnemy;
	this->stun = stun;

	if (fire) FireProjectile(speed);
}

void Projectile::FireProjectile(fPoint speed)
{
	b2Vec2 s = { speed.x, speed.y };
	pBody->body->SetLinearVelocity(s);
}

void Projectile::OnCollisionEnter(PhysBody* col)
{
	pendingToDelete = true;
}
