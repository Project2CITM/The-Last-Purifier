#ifndef _SPELL_OBJECT_
#define _SPELL_OBJECT_

#include "GameObject.h"
#include "SpellInfo.h"

class SpellObject : public GameObject
{
public:
	SpellObject(iPoint position, SpellID id, int level);

	void OnCollisionEnter(PhysBody* col) override;
private:
	SpellInfo info;
};
#endif  

