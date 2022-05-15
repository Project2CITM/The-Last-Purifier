#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include "RoomManager.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "List.h"

#define DEFAULT_WIDTH 150
#define DEFAULT_HEIGHT 100

#define RECT_WIDTH 30
#define RECT_HEIGHT 20

class MiniMap
{
public:
	MiniMap()
	{
		this->app = Application::GetInstance();
	}

	~MiniMap();

	void Init(bool isHub, List<Room*>* room = nullptr);
	void SetScale(int scale);
	void SetAlpha(int alpha);
	void MiniMapPrint(iPoint pos, iPoint playerPos);

private:

	Application* app = nullptr;
	bool isHub = true;
	SDL_Texture* textureHub = nullptr;
	SDL_Texture* icon = nullptr;
	int scale = 1;
	int alpha = 100;
	List<Room*>* rooms;
};

#endif //__MINIMAP_H__