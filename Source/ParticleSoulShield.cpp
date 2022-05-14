#include "ParticleSoulShield.h"
#include "ModuleTextures.h"
#include "PlayerController.h"

ParticleSoulShield::ParticleSoulShield(iPoint position, GameObject* player, float life, float delay, iPoint velocity, std::string name) :Particle(position, life, delay, velocity, name)
{
	renderObjects[0].InitAsTexture(app->textures->Load("Sprites/Player/Revenant/soulShield.png"), position, { 0,0,0,0 });
	renderObjects[0].layer = 2.0f;
	renderObjects[0].orderInLayer = 4.0f;
	this->player = player;
}

ParticleSoulShield::~ParticleSoulShield()
{
}

void ParticleSoulShield::PreUpdate()
{

	Particle::PreUpdate();

	if (player == nullptr)return;

	if (player->pendingToDelete)return;

	if (pendingToDelete)return;

	iPoint particleOffset = { -8,-20 };

	this->SetPosition(player->GetPosition() + particleOffset);

}

void ParticleSoulShield::PostUpdate()
{
	GameObject::PostUpdate();
}
