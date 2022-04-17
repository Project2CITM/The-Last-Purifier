#include "ParticleAttackKaboom.h"

ParticleAttackKaboom::ParticleAttackKaboom(iPoint position) :Particle(position, 1, 0)
{
	InitRenderObjectWithXml("kaboomExplotion");

	for (int i = 0; i < 7; i++)
	{
		anim.PushBack({ i * 32,0,32,32 });
	}

	anim.loop = false;

	anim.speed = 0.15f;

	anim.hasIdle = false;
}

ParticleAttackKaboom::~ParticleAttackKaboom()
{
}

void ParticleAttackKaboom::PostUpdate()
{
	Particle::PostUpdate();

	if (anim.HasFinished()) pendingToDelete = true;
}
