#include "EnemyDummy.h"
#include "ModulePhysics.h"

EnemyDummy::EnemyDummy(iPoint pos)
{
	pBody = app->physics->CreateRectangle(pos, 10, 14, this);
}
