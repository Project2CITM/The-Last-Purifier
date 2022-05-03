#include "ParticleExplodeEkriksi.h"
#include "ModuleTextures.h"

ParticleExplodeEkriksi::ParticleExplodeEkriksi(iPoint position, float life, float delay, iPoint velocity, std::string name) :Particle(position, life, delay, velocity, name)
{
	for (int i = 0; i < 10; i++)
	{
		this->anim.PushBack({ 0,128 * i,128,128 });
	}
	this->anim.loop = false;
	this->anim.hasIdle = false;
	this->anim.duration = 0.0320f;

	renderObjects[0].InitAsTexture(app->textures->Load("Assets/Sprites/Player/Sage/ekriskis2.png"), position, { 0,0,0,0 });
}

ParticleExplodeEkriksi::~ParticleExplodeEkriksi()
{
}