#pragma once

#include "External/SDL/include/SDL.h"
#include "Point.h"

#define DEG_TO_RAD(a) a * PI / 180.0f
#define RAD_TO_DEG(a) a * 180.0f / PI

class SingleParticle
{
private:

	uint life = 0, startLife = 0;

	union ParticleInfo
	{
		// This struct holds the state of the particle when 
		// it's being update (it's still alive).
		struct ParticleState
		{
			fPoint pos = { 0.0f, 0.0f };
			fPoint vel = { 0.0f, 0.0f };
			float radius = 0.0f;
			float start_radius = 0.0f;
			float ageRatio = 0.0f;
			float angle = 0.0f;
			SDL_Rect pRect = { 0, 0, 0, 0 };
			SDL_Rect rectSize = { 0, 0, 0, 0 };
			SDL_Color startColor = { 0, 0, 0, 0 };
			SDL_Color endColor = { 0, 0, 0, 0 };
			SDL_BlendMode blendMode = SDL_BLENDMODE_NONE;
			float t;

			ParticleState() {}

		} pLive;

		// If the particle is dead, then the 'next' member comes 
		// into play and the struct it's not used. This pointer
		// called 'next' holds a pointer to the next available 
		// particle after this one.
		SingleParticle* next;

		ParticleInfo() {}
	} pState;

public:

	SingleParticle();
	void Init(fPoint pos, float speed, float angle, float start_radius, uint life, SDL_Rect textureRect, SDL_Color startColor, SDL_Color endColor, SDL_BlendMode blendMode);

	void Update(float dt);
	void Draw();

	bool IsAlive();

	SingleParticle* GetNext();
	void SetNext(SingleParticle* next);

	SDL_Color RgbInterpolation(SDL_Color startColor, SDL_Color endColor, float timeStep);
};

