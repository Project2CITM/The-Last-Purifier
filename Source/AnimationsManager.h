#pragma once

#include "List.h"
#include "Animation.h"

struct AnimationObject
{
	Animation* anim;
	bool canBeInterrupted = true;
	int preferenceOrder = 0;
};

class AnimationsManager
{
public:
	AnimationsManager();

	uint AddAnimationObject(AnimationObject* animObj);

	uint AddAnimationObject(Animation* anim, bool interrputed = true, int preference = 0);

	void DoAnimation(uint animation);

	void CleanUp();

	List<AnimationObject*> animationObjects;

	uint currentAnimation = 0;
};

