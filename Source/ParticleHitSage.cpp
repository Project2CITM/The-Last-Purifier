#include "ParticleHitSage.h"
#include "ModuleTextures.h"

ParticleHitSage::ParticleHitSage(iPoint position, float life, float delay, iPoint velocity, std::string name) :Particle(position, life, delay, velocity, name)
{
	for (int i = 0; i < 7; i++)
	{
		this->anim.PushBack({ 32 * i,0,32,32 });
	}
	this->anim.loop = false;
	this->anim.hasIdle = false;
	this->anim.duration = 0.0320f;

	renderObjects[0].InitAsTexture(app->textures->Load("Sprites/Player/Sage/basicAttackSageImpact.png"), position, { 0,0,0,0 });
}

ParticleHitSage::~ParticleHitSage()
{
}
