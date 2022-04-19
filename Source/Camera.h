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

	void SetPosition(iPoint pos);

	void ReleaseTarget();

	/// <summary>
	/// Camera shake effect
	/// </summary>
	/// <param name="power">pixel</param>
	/// <param name="time">frame</param>
	/// <param name="attenuate">pixel</param>
	void Shake(int power, int time = 20, int attenuate = 1);

private:

	void DoShake();

	iPoint GetCenter();

	Application* app = nullptr;

	GameObject* target = nullptr;

	// use for offset with target
	int pivotX = 0, pivotY = 0;

	int distanceFromTarget = 0;

	bool debug = false;

	int mapHeight = 0, mapWidth = 0;

	// ShakeEffect
	int shakePower = 0, shakeTime = 0, shakeAttenuate = 0;

public:
	int x = 0;
	int y = 0;

	int cameraSpeed = 20;

	// mas grande, mas lento se mueve la camera
	int cameraDelayInRange = 10;
	int cameraDelayOutRange = 3;

	// distancia donde la camera empieza a perseguirte
	int followDistance = 100;

	// decidir si se mueve o no
	int moveX = 1, moveY = 1;
};

#endif // !__CAMERA_H__