#ifndef __ANIMAION_H__
#define __ANIMAION_H__

#include <iostream>
#include "External/SDL/include/SDL_rect.h"

#define MAX_FRAMES 30

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	bool hasIdle = true;

private:
	SDL_Rect frames[MAX_FRAMES];
	int loopCount = 0;
	int totalFrames = 0;
	float current_frame = 0.0f;
	int last_frame = 0;

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
	}

	void Reset()
	{
		current_frame = 0;
	}

	void Update()
	{
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
			current_frame += speed;
			if (current_frame >= last_frame && loop) current_frame = 0;
		}
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
};

#endif // !__ANIMAION_H__