#ifndef _PLAYER_SHADOW_
#define _PLAYER_SHADOW_

#include "GameObject.h"

class PlayerShadow : public GameObject
{
public:
	PlayerShadow(GameObject* follow);

	void PreUpdate() override;

	void PostUpdate() override;

private:
	GameObject* follow = nullptr;
	iPoint offset = { -4,8 };
};
#endif	
