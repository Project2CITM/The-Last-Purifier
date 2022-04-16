#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "GameObject.h"
#include "Animation.h"

class Particle :public GameObject
{
public:

	Particle(iPoint position,float life = 0, float delay = 0, iPoint velocity = { 0,0 }, std::string name = "Particle");

	~Particle();

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	virtual void OnCollisionEnter(PhysBody* col) override;

	virtual void OnCollisionExit(PhysBody* col) override;

	void CleanUp() override;

protected:

	float life = 0; // second

	float count = 0; // second

	float delay = 0; // second

	iPoint velocity = { 0,0 };

	Animation anim;
	
};

#endif // !__PARTICLE_H__