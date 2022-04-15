#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "GameObject.h"

class SpellSpawnManager;

class Enemy :  public GameObject
{
public: 
	
	Enemy(std::string name);

	~Enemy();

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	void OnCollisionEnter(PhysBody* col) override;

	void OnCollisionExit(PhysBody* col) override;

	virtual void Hit(int damage);

	virtual void Die();

protected:
	SpellSpawnManager* spawnManager = nullptr;

	int health = 40;

	int moveSpeed = 5;
};

#endif // !__ENEMY_H__
