#ifndef __COLUMN_H__
#define __COLUMN_H__

#include "Enemy.h"

class Timer;

class Column : public Enemy
{
public:
	Column(iPoint pos);

	~Column();

	void PreUpdate() override;

	void PostUpdate() override;

	void OnCollisionEnter(PhysBody* col) override;

	void DisableCollisions() override;
private:
	SDL_Rect sections[2];

	bool destroyed = false;

	int destroyedMS = 500;

	Timer* columnTimer = nullptr;
};

#endif // !__COLUMN_H__