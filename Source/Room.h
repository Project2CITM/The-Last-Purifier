#ifndef __ROOM_H__
#define __ROOM_H__

#include "GameObject.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

#define MAX_ROOM_TILES_COLUMNS 81
#define MAX_ROOM_TILES_ROWS 35
#define TILE_SIZE 1 //16

enum class DoorOrientations {
	RIGHT,
	BOTTOM,
	LEFT,
	TOP
};

class Door: public GameObject
{
public:
	Door(iPoint position, iPoint size, DoorOrientations orient) :GameObject("door")
	{
		this->position = position;

		this->size = size;

		orientation = orient;
	};
	~Door()
	{};

	void CreateCollider()
	{
		pBody = Application::GetInstance()->physics->CreateRectangle({ position.x,position.y }, size.x, size.y, this, b2BodyType::b2_staticBody);
	};

	void DestroyCollider()
	{
		if (pBody != nullptr)
		{
			delete pBody;
			pBody = nullptr;
		}
	};

	DoorOrientations orientation;

	iPoint size = { 1,1 };

	bool open = true;
};

class Room
{
public:
	Room()
	{
		this->app = Application::GetInstance();
	}

	void CloseDoors();

	void OpenDoors();

	void DrawRoom();

	void CleanUp();

	iPoint GetDoorPos(DoorOrientations orient);

	iPoint GetDoorSize(DoorOrientations orient);

private:
	iPoint doorPos[4] = { {80,18},{39,34},{0,18},{39,0} };
	iPoint doorSize[4] = { {1,2},{3,1},{1,2},{3,6} };

public:
	short id = 0;
	Application* app = nullptr;
	PhysBody* wallColliders[4];
	List<PhysBody*> colliders;
	List<Door*> doors;
	iPoint roomPosition; 	//in map, not pixels
	SDL_Texture* roomTexture = nullptr;
};

#endif //_ROOM_H_