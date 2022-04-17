#ifndef __WORM_H__
#define __WORM_H__

#include "Enemy.h"

enum class WormState
{
	IDLE = 0,
	ATTACK,
	HIT,
	DIE,
	MAX
};

class Worm : public Enemy
{
public:

	Worm(iPoint position);

	~Worm();

public:


};

#endif // !__WORM_H__