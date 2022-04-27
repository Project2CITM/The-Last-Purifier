#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include "RoomManager.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

#define DEFAULT_WIDTH 300
#define DEFAULT_HEIGHT 200

enum class RoomState {
	NO_ROOM,
	UNDONE,
	DONE
};

enum class MiniMapState {
	DUNGEON,
	HUB,
};

class MiniMap
{
public:
	MiniMap()
	{
		this->app = Application::GetInstance();
	}

	void Init(bool hub);
	void Update();
	void SetScale(float scale);
	void MiniMapPrint(iPoint pos);

private:
	
	Application* app = nullptr;
	bool hubState;
	int scale = 1.0f;
	int alpha = 255;
	List<SDL_Rect> rects;
	//RoomState rooms[MAX_ROOMS_COLUMNS][MAX_ROOMS_ROWS];
	//int roomIds[MAX_ROOMS_COLUMNS][MAX_ROOMS_ROWS];
};

#endif //__MINIMAP_H__