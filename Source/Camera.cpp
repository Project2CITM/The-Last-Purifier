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

	// Update Y
	int targetPosY = target->GetPosition().y * app->window->scale;
	targetPosY = (targetPosY - pivotY) * moveY;

	int distance = abs(targetPosY - y);

	if (distance > followDistance)
	{
		targetPosY > y ? y += distance / (cameraDelay / 3) : targetPosY < y ? y -= distance / (cameraDelay / 3) : y = y;
	}
	else if (distance != 0)
	{
		targetPosY > y ? y += distance / cameraDelay : targetPosY < y ? y -= distance / cameraDelay : y = y;
	}

	// Update X
	int targetPosX = target->GetPosition().x * app->window->scale;
	targetPosX = (targetPosX - pivotX) * moveX;

	distance = abs(targetPosX - x);

	if (distance > followDistance)
	{
		targetPosX > x ? x += distance / (cameraDelay / 3) : targetPosX < x ? x -= distance / (cameraDelay / 3) : x = x;
	}
	else if (distance != 0)
	{
		targetPosX > x ? x += distance / cameraDelay : targetPosX < x ? x -= distance / cameraDelay : x = x;
	}

	// Limite de camera
	//y < 0 ? y = 0 : y > mapHeight ? y = mapHeight : y = y;
	//x < 0 ? x = 0 : x > mapHeight ? x = mapHeight : x = x;
}

void Camera::SetTarget(GameObject* target)
{
	this->target = target;
}

void Camera::ReleaseTarget()
{
	target = nullptr;
	x = y = 0;
}

iPoint Camera::GetCenter()
{
	return {x + pivotX, y + pivotY };
}