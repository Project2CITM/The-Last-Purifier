#ifndef __ROOMMANAGER_H__
#define __ROOMMANAGER_H__

#include "Room.h"
#include "GameObject.h"
#include "Timer.h"
#include "ModuleTextures.h"
#include "MapLoader.h"

#define MAX_ROOMS_COLUMNS 8
#define MAX_ROOMS_ROWS 8

class RoomManager{
	
public:
	RoomManager()
	{
		this->app = Application::GetInstance();
	}

	void Start();
	void Update(iPoint playerPos);
	void PostUpdate();
	void CleanUp();

	void GenerateMap(short RoomNumber);
	int CheckAdjacentSpace(Room* r);
	int CheckAdjacentSpace(iPoint p);
	void CreateDoors();
	Room* CreateRoom(iPoint mapPosition, short mapId = 0);

	void DrawRooms();
	void DrawDoors();

public:
	Application* app = nullptr;
	List<Room*> rooms;
	Room* roomPositions[MAX_ROOMS_COLUMNS][MAX_ROOMS_ROWS];
	iPoint bossRoom = iPoint(-1, -1);

private:
	SDL_Texture* doorTopTexture = nullptr;
	SDL_Texture* doorBotTexture = nullptr;
	SDL_Texture* doorSpikeTexture = nullptr;

	SDL_Texture* wallTexture[3] = {nullptr, nullptr, nullptr};

	MapLoader* mapLoader = nullptr;
};

#endif //__ROOMMANAGER_H_