#ifndef  __CAMERA_H__
#define  __CAMERA_H__

#include "Point.h"

class Application;
class GameObject;

class Camera
{
public:
	Camera(GameObject* target = nullptr);

	void Init(GameObject* target, int width, int height);

	void Start();

	void Update();

	void UpdatePosition();

	void SetTarget(GameObject * target);

	void ReleaseTarget();

private:

	iPoint GetCenter();

	Application* app = nullptr;

	GameObject* target = nullptr;

	// use for offset with target
	int pivotX = 0, pivotY = 0;

	int distanceFromTarget = 0;

public:
	int x = 0;
	int y = 0;

	int cameraSpeed = 5;

	// mas grande, mas lento se mueve la camera
	int cameraDelay = 0;

	// decidir si se mueve o no
	int moveX = 0, moveY = 1;

	int mapHeight = 0, mapWidth = 0;
};

#endif // !__CAMERA_H__