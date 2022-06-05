#ifndef __ANIMAION_H__
#define __ANIMAION_H__

#include <iostream>
#include "External/SDL/include/SDL_rect.h"
#include "Timer.h"

#define MAX_FRAMES 30

class Animation
{
public:
	bool loop = true;
	float duration = 1.0f;
	bool hasIdle = true;

private:
	SDL_Rect frames[MAX_FRAMES];
	int loopCount = 0;
	int totalFrames = 0;
	float current_frame = 0.0f;
	int last_frame = 0;
	Timer animationTimer;
	float durationCounter = 0;

public:

	float getCurrentFrameF()
	{
		return current_frame;
	}

	int getCurrentFrameI()
	{
		return (int)current_frame;
	}

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
		totalFrames++;
	}

	void Reset()
	{
		current_frame = 0;
	}

	void Update()
	{
		animationTimer.Update();
		// if the animation doesn't loop and has reached its end, don't update
		if (!loop && current_frame >= last_frame-1)
		{
			current_frame = last_frame-1;
			return;
		}

		if (hasIdle) {
			current_frame = 0;
		}
		else {
			durationCounter += animationTimer.getDeltaTime();
			if (durationCounter >= duration) 
			{ 
				current_frame++;
				durationCounter = 0;
			}
			if (current_frame >= last_frame && loop) current_frame = 0;
		}
		animationTimer.Reset();
	}

	SDL_Rect& GetCurrentFrame()
	{
		return frames[(int)current_frame];
	}

	SDL_Rect& getFrame(int frame) {
		return frames[frame];
	}

	bool HasFinished()
	{
		if (current_frame >= last_frame - 1 && !loop)
		{
			return true;
		}

		return false;
	}

	int GetTotalFrames()
	{
		return totalFrames;
	}

	int size() {
		return last_frame;
	}

	void EmptyAnimation()
	{
		loop = true;
		duration = 1.0f;
		hasIdle = true;
		for (int i = 0; i < MAX_FRAMES; i++)
		{
			frames[i] = { 0,0,0,0 };
		}
		loopCount = 0;
		totalFrames = 0;
		current_frame = 0.0f;
		last_frame = 0;
		durationCounter = 0;
	}
};

#endif // !__ANIMAION_H__