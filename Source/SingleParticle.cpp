#include "SingleParticle.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"

SingleParticle::SingleParticle() :life(0), startLife(0) { }

void SingleParticle::Init(fPoint pos, float speed, float angle, float start_radius, uint life, SDL_Rect textureRect, SDL_Color startColor, SDL_Color endColor, SDL_BlendMode blendMode)
{
	pState.pLive.pos = pos;
	pState.pLive.vel.x = speed * cos(DEG_TO_RAD(angle));
	pState.pLive.vel.y = -speed * sin(DEG_TO_RAD(angle));
	this->life = this->startLife = life;
	pState.pLive.radius = pState.pLive.start_radius = start_radius;
	pState.pLive.pRect = pState.pLive.rectSize = textureRect;
	pState.pLive.startColor = startColor;
	pState.pLive.endColor = endColor;
	pState.pLive.blendMode = blendMode;
	pState.pLive.t = 0.0f;
}

void SingleParticle::Update(float dt)
{
	life--;

	pState.pLive.ageRatio = (float)this->life / (float)this->startLife;
	float alpha = (pState.pLive.ageRatio * 255.0f);
	pState.pLive.startColor.a = alpha;
	pState.pLive.radius = pState.pLive.start_radius * pState.pLive.ageRatio;
	pState.pLive.rectSize.w = pState.pLive.rectSize.h = pState.pLive.radius;

	pState.pLive.pos.x += pState.pLive.vel.x * dt;
	pState.pLive.pos.y += pState.pLive.vel.y * dt;
}

void SingleParticle::Draw()
{
	SDL_Rect rectTest = { (int)pState.pLive.start_radius, (int)pState.pLive.start_radius };
	float centerX = pState.pLive.pos.x + ((rectTest.w - pState.pLive.rectSize.w) / 2);
	float centerY = pState.pLive.pos.y + ((rectTest.h - pState.pLive.rectSize.h) / 2);

	
	// SDL_Texture* texture, float alpha, int x, int y, const SDL_Rect* section, const SDL_Rect* rectSize, float speed, double angle, int pivot_x, int pivot_y
	//Application::GetInstance()->renderer->BlitParticle(Application::GetInstance()->psystem->GetParticleAtlas(), (int)centerX, (int)centerY, &pState.pLive.pRect, &pState.pLive.rectSize, RgbInterpolation(pState.pLive.startColor, pState.pLive.endColor, pState.pLive.t), pState.pLive.blendMode);
	//Application::GetInstance()->renderer->AddTextureRenderQueue(Application::GetInstance()->psystem->GetParticleAtlas(), { (int)centerX, (int)centerY }, pState.pLive.pRect, 0.25f, 4, 10);
	Application::GetInstance()->renderer->BlitParticle(Application::GetInstance()->psystem->GetParticleAtlas(), { (int)centerX, (int)centerY }, pState.pLive.pRect, 0.25f, 3, 20, RgbInterpolation(pState.pLive.startColor, pState.pLive.endColor, pState.pLive.t), pState.pLive.blendMode);

	pState.pLive.t += (1.0f / (float)startLife);

	if (pState.pLive.t >= 1.0f)
		pState.pLive.t = 0.0f;
}

bool SingleParticle::IsAlive()
{
	return (life > 0);
}

SingleParticle* SingleParticle::GetNext()
{
	return pState.next;
}

void SingleParticle::SetNext(SingleParticle* next)
{
	pState.next = next;
}

SDL_Color SingleParticle::RgbInterpolation(SDL_Color startColor, SDL_Color endColor, float timeStep)
{
	SDL_Color finalColor;

	finalColor.r = startColor.r + (endColor.r - startColor.r) * timeStep;
	finalColor.g = startColor.g + (endColor.g - startColor.g) * timeStep;
	finalColor.b = startColor.b + (endColor.b - startColor.b) * timeStep;
	finalColor.a = startColor.a + (endColor.a - startColor.a) * timeStep;

	return finalColor;
}