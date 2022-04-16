#include "Projectile.h"
#include "ModulePhysics.h"
#include "DamageArea.h"
#include "ParticleAttackSage.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "Particle.h"
#include "ParticleHitSage.h"

Projectile::Projectile(std::string name, iPoint position, fPoint speed, int damage, bool fire, bool stun, bool isEnemy) : GameObject(name, name)
{
	this->damage = damage;
	this->isEnemy = isEnemy;
	this->stun = stun;

	damageArea = new DamageArea(position, 4, 4, &this->damage, &this->stun);
	pBody = app->physics->CreateRectangle(position, 4, 4, this);
	b2Filter filter;
	filter.categoryBits = app->physics->PROJECTILE_LAYER;

	if (isEnemy) filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER & ~app->physics->PROJECTILE_LAYER & ~app->physics->TRIGGER_LAYER;
	else filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->PLAYER_LAYER & ~app->physics->PROJECTILE_LAYER & ~app->physics->TRIGGER_LAYER;

	pBody->body->GetFixtureList()[0].SetFilterData(filter);
	damageArea->pBody->body->GetFixtureList()[0].SetFilterData(filter);

	if (fire) FireProjectile(speed);

	

	renderObjects[0].InitAsTexture(app->textures->Load("Assets/Sprites/Player/Sage/basicAttackSageDuring.png"), position, { 0,0,0,0 });

	
	// Particle Effect

	for (int i = 0; i < 8; i++)
	{
		this->anim.PushBack({ 32 * i,0,32,32 });
	}
	this->anim.loop = true;
	this->anim.hasIdle = false;
	this->anim.speed = 0.5f;

	new ParticleAttackSage({ position.x - 20,position.y - 15 }, 0.1f, 0.01f, { 1,0 });
	
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

	new ParticleHitSage({ position.x - 20,position.y - 15 }, 1.5f);

}

void Projectile::PostUpdate()
{
	anim.Update();

	renderObjects[0].section = anim.GetCurrentFrame();

	renderObjects[0].destRect.x = GetDrawPosition().x - 20;
	renderObjects[0].destRect.y = GetDrawPosition().y - 15;
	
	app->renderer->AddRenderObjectRenderQueue(renderObjects[0]);

}
