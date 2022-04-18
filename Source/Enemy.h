#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "GameObject.h"
#include "Player.h"

class SpellSpawnManager;
class Player;

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

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;

	virtual void Hit(int damage);

	virtual void Die(bool spawnPower = false);

	int GetDamage()
	{
		return damage;
	}

protected:
	SpellSpawnManager* spawnManager = nullptr;

	int health = 40;

	int damage = 10;

	int moveSpeed = 5;

private:
	Player* player = nullptr;

};

#endif // !__ENEMY_H__
