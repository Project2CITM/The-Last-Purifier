#include "AnimationsManager.h"

AnimationsManager::AnimationsManager()
{
}

uint AnimationsManager::AddAnimationObject(AnimationObject* animObj)
{
	animationObjects.add(animObj);
	return animationObjects.count()-1;
}

uint AnimationsManager::AddAnimationObject(Animation* anim, bool interrputed, int preference)
{
	AnimationObject* animObj = new AnimationObject();

	animObj->anim = anim;
	animObj->canBeInterrupted = interrputed;
	animObj->preferenceOrder = preference;

	animationObjects.add(animObj);
    return animationObjects.count() - 1;
}

void AnimationsManager::DoAnimation(uint animation)
{
	// If the current animation can't be interrupted and hasn't finished yet, return
	if (!animationObjects[currentAnimation]->canBeInterrupted && !animationObjects[currentAnimation]->anim->HasFinished()) return;

	currentAnimation = animation;

}

void AnimationsManager::CleanUp()
{
	animationObjects.clearPtr();
}
