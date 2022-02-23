#include "Camera.h"
#include "Application.h"
#include "GameObject.h"

Camera::Camera(Application* app, GameObject* target)
{
	App = app;
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
	pivotX = App->window->width / 2;
	pivotY = App->window->height / 2;
}

void Camera::Update()
{
	UpdatePosition();

	//if (!App->debug->debugCamera) return;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		y -= cameraSpeed;
		//printf_s("Camera_X: %d, Camera_Y: %d\n", x, y);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		y += cameraSpeed;
		//printf_s("Camera_X: %d, Camera_Y: %d\n", x, y);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		x -= cameraSpeed;
		//printf_s("Camera_X: %d, Camera_Y: %d\n",x, y);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		x += cameraSpeed;
		//printf_s("Camera_X: %d, Camera_Y: %d\n", x, y);
	}
}

void Camera::UpdatePosition()
{
	if (target == nullptr || App->scene->isChangingScene) return;

	// Update Y
	int targetPosY = target->GetPosition().y * App->window->scale * App->renderer->zoom;
	targetPosY = (targetPosY - pivotY * App->renderer->zoom) * moveY;

	int distance = abs(targetPosY - y);

	if (distance > 200)
	{
		targetPosY > y ? y += distance / (cameraDelay / 3) : targetPosY < y ? y -= distance / (cameraDelay / 3) : y = y;
	}
	else if (distance != 0) targetPosY > y ? y += distance / cameraDelay : targetPosY < y ? y -= distance / cameraDelay : y = y;

	// Update X
	int targetPosX = target->GetPosition().x * App->window->scale * App->renderer->zoom;
	targetPosX = (targetPosX - pivotX * App->window->scale) * moveX;

	distance = abs(targetPosX - x);

	if (distance != 0) targetPosX > x ? x += distance / cameraDelay : targetPosX < x ? x -= distance / cameraDelay : x = x;

	// Limite de camera
	y < 0 ? y = 0 : y > mapHeight ? y = mapHeight : y = y;
	x < 0 ? x = 0 : x > mapHeight ? x = mapHeight : x = x;
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