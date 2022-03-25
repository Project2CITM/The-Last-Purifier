#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "GameObject.h"

class Enemy :  public GameObject
{
public: 
	
	Enemy();

	~Enemy();

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	void OnCollisionEnter(PhysBody* col) override;

	void OnCollisionExit(PhysBody* col) override;
};

#endif // !__ENEMY_H__
