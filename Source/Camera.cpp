#include "Camera.h"
#include "GameObject.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"

Camera::Camera(GameObject* target)
{
	app = Application::GetInstance();
	this->target = target;
}

void Camera::Init(GameObject* target, int width, int height)
{
	this->target = target;
	mapWidth = width;
	mapHeight = height;
}

void Camera::Start()
{
	pivotX = app->window->width / 2;
	pivotY = app->window->height / 2;
}

void Camera::Update()
{
	UpdatePosition();

	//if (!app->debug->debugCamera) return;

	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		debug = !debug;
	}

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		y -= cameraSpeed;
		//printf_s("Camera_X: %d, Camera_Y: %d\n", x, y);
	}
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		y += cameraSpeed;
		//printf_s("Camera_X: %d, Camera_Y: %d\n", x, y);
	}
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		x -= cameraSpeed;
		//printf_s("Camera_X: %d, Camera_Y: %d\n",x, y);
	}
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		x += cameraSpeed;
		//printf_s("Camera_X: %d, Camera_Y: %d\n", x, y);
	}
}

void Camera::UpdatePosition()
{
	if (target == nullptr || app->scene->isChangingScene || debug) return;

	// Shake Effect
	if (shakeTime > 0 && shakePower > 0) DoShake();

	// If delay = 0;
	if (cameraDelayInRange == 0 || cameraDelayOutRange == 0)
	{
		x = target->GetPosition().x * app->window->scale;
		y = target->GetPosition().y * app->window->scale;
		return;
	}

	// Update Y
	int targetPosY = target->GetPosition().y * app->window->scale;
	targetPosY = (targetPosY - pivotY) * moveY;

	int distance = abs(targetPosY - y);

	if (distance > followDistance)
	{
		targetPosY > y ? y += distance / cameraDelayOutRange : targetPosY < y ? y -= distance / cameraDelayOutRange : y = y;
	}
	else if (distance != 0)
	{
		targetPosY > y ? y += distance / cameraDelayInRange : targetPosY < y ? y -= distance / cameraDelayInRange : y = y;
	}

	// Update X
	int targetPosX = target->GetPosition().x * app->window->scale;
	targetPosX = (targetPosX - pivotX) * moveX;

	distance = abs(targetPosX - x);

	if (distance > followDistance)
	{
		targetPosX > x ? x += distance / cameraDelayOutRange : targetPosX < x ? x -= distance / cameraDelayOutRange : x = x;
	}
	else if (distance != 0)
	{
		targetPosX > x ? x += distance / cameraDelayInRange : targetPosX < x ? x -= distance / cameraDelayInRange : x = x;
	}

	// Limite de camera
	//y < 0 ? y = 0 : y > mapHeight ? y = mapHeight : y = y;
	//x < 0 ? x = 0 : x > mapHeight ? x = mapHeight : x = x;
}

void Camera::SetTarget(GameObject* target)
{
	this->target = target;
}

void Camera::SetPosition(iPoint pos)
{
	int targetPosX = pos.x * app->window->scale;
	targetPosX = (targetPosX - pivotX) * moveX;

	x = targetPosX;

	int targetPosY = pos.y * app->window->scale;
	targetPosY = (targetPosY - pivotY) * moveY;

	y = targetPosY;
}

void Camera::ReleaseTarget()
{
	target = nullptr;
	x = y = 0;
}

void Camera::Shake(int power, int time, int attenuate)
{
	this->shakePower = power;
	this->shakeTime = time;
	this->shakeAttenuate = attenuate;
}

void Camera::DoShake()
{
	int currentShakeX = rand() % shakePower - (shakePower / 2);
	int currentShakeY = rand() % shakePower - (shakePower / 2);

	x += currentShakeX;
	y += currentShakeY;

	shakePower -= shakeAttenuate;
	shakeTime--;
}

iPoint Camera::GetCenter()
{
	return {x + pivotX, y + pivotY };
}