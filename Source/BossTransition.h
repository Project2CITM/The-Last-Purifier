#ifndef __BOSS_TRANSITION_H__
#define __BOSS_TRANSITION_H__

#include "RenderObject.hpp"

class Application;

class BossTransition
{
public:

	BossTransition();

	~BossTransition();

	void Update();

	void PostUpdate();

public:

	RenderObject barTop;

	int barTopMoveSpeed = 20;

	RenderObject barBottom;

	int barBottomMoveSpeed = 20;

	RenderObject boss;

	int bossMoveSpeed = 20;

	int finishCoolDown = 200; //frames

	bool transitionStart = false;

	bool transitionEnd = false;

private:

	Application* app = nullptr;

};

#endif // !__BOSS_TRANSITION_H__