#ifndef __ROOMMANAGER_H__
#define __ROOMMANAGER_H__

#include "Room.h"
#include "GameObject.h"
#include "Timer.h"
#include "ModuleTextures.h"
#include "MapLoader.h"
#include "MapSave.h"

#define MAX_ROOMS_COLUMNS 8
#define MAX_ROOMS_ROWS 8

class Trigger;
class MiniMap;

class RoomManager{
	
public:
	RoomManager()
	{
		this->app = Application::GetInstance();
	}

	void Start();
	void PreUpdate(iPoint playerPos);
	void Update(iPoint playerPos);
	void PostUpdate(iPoint playerPos);
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

private:
	SDL_Texture* doorTopTexture = nullptr;
	SDL_Texture* doorBotTexture = nullptr;
	SDL_Texture* doorSpikeTexture = nullptr;
	SDL_Texture* wallTexture[3] = {nullptr, nullptr, nullptr};

	Trigger* exitTrigger = nullptr;

	MapLoader* mapLoader = nullptr;
	MapSave* mapSave = nullptr;
	MiniMap* miniMap = nullptr;
};

#endif //__ROOMMANAGER_H_