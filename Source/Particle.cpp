#include "Particle.h"

Particle::Particle(iPoint position,float life, float delay, iPoint velocity, std::string name):GameObject("partiicle","Particle")
{
	this->life = life;
	this->delay = delay;
	this->velocity = velocity;
	this->name = name;
	this->position = position;
}

Particle::~Particle()
{
}

void Particle::Start()
{
}

void Particle::PreUpdate()
{
	if (life > 0)
	{
		if (count >= life) pendingToDelete = true;
	}
	else if (anim.HasFinished())
	{
		pendingToDelete = true;
	}
}

void Particle::Update()
{
	// /1000 to convert ms in second
	if (delay >= 0) delay -= (app->frameTime);

	else if (life >= 0) count += (app->frameTime);

	// move with velocity
	position += velocity;

	// Update collision pos with gameobject position
	GameObject::Update();
}

void Particle::PostUpdate()
{
	if (delay <= 0)
	{
		if (anim.GetTotalFrames() > 0)
		{
			anim.Update();

			renderObjects[0].section = anim.GetCurrentFrame();
		}

		//Draw textures
		GameObject::PostUpdate();
	}
}

void Particle::OnCollisionEnter(PhysBody* col)
{
}

void Particle::OnCollisionExit(PhysBody* col)
{
}

void Particle::CleanUp()
{
}
