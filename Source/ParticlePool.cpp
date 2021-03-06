#include "ParticlePool.h"
#include "ParticleEmissor.h"
#include <assert.h>

// This pool constructor sets our particles to available
ParticlePool::ParticlePool(ParticleEmissor* emitter)
{
	// Fill the pool according to poolSize needed for the emitter
	poolSize = emitter->GetPoolSize();
	particleArray = new SingleParticle[poolSize];

	// The first particle is available
	firstAvailable = &particleArray[0];

	// Each particle points to the next one
	for (int i = 0; i < poolSize - 1; i++)
		particleArray[i].SetNext(&particleArray[i + 1]);

	// The last particles points to nullptr indicating the end of the list
	particleArray[poolSize - 1].SetNext(nullptr);
}

ParticlePool::~ParticlePool()
{
	delete[] particleArray;
	particleArray = nullptr;
}

void ParticlePool::Generate(fPoint pos, float speed, float angle, float start_radius, uint life, SDL_Rect textureRect, SDL_Color startColor, SDL_Color endColor, SDL_BlendMode blendMode)
{
	// Check if the pool is not full
	assert(firstAvailable != nullptr);

	// Remove it from the available list
	SingleParticle* newParticle = firstAvailable;
	firstAvailable = newParticle->GetNext();

	// Inizialize new alive particle
	newParticle->Init(pos, speed, angle, start_radius, life, textureRect, startColor, endColor, blendMode);
}

bool ParticlePool::Update(float dt)
{
	bool ret = false;

	for (int i = 0; i < poolSize; i++)
	{
		if (particleArray[i].IsAlive())
		{
			particleArray[i].Update(dt);
			particleArray[i].Draw();
			ret = true;
		}
		else // if a particle dies it becomes the first available one in the pool
		{
			// Add this particle to the front of the vector
			particleArray[i].SetNext(firstAvailable);
			firstAvailable = &particleArray[i];
		}
	}

	return ret;
}