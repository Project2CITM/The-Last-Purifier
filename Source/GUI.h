#ifndef __GUI_H__
#define __GUI_H__

#include "External/SDL/include/SDL.h"
#include "Point.h"

class Application;

enum class UIShape
{
	CIRCLE,
	BOX
};

struct RectangleShape
{
	int w = 0;
	int h = 0;
};

struct CircleShape
{
	int radius = 0;
};

class GUI
{
public:
	GUI(Application* app);

	virtual ~GUI();

	void InitAsBox(int x, int y, int w, int h);

	void InitAsCircle(int x, int y, int radius);

	virtual void Update();

	virtual void PostUpdate();

protected:
	bool CheckOnMouse();

protected:
	Application* _app = nullptr;

	iPoint position = { 0,0 };

	// Box Shape
	RectangleShape boxShape;

	// Circle Shape
	CircleShape circleShape;

	SDL_Color defaultColor = { 255,255,255,255 };

	UIShape currentShape = UIShape::CIRCLE;
};

#endif // !__GUI_H__