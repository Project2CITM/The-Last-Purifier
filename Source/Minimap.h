#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include "RoomManager.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

#define DEFAULT_WIDTH 300
#define DEFAULT_HEIGHT 200

#define RECT_WIDTH 75
#define RECT_HEIGHT 50

class MiniMap
{
public:
	MiniMap()
	{
		this->app = Application::GetInstance();
	}

	~MiniMap();

	void Init(bool isHub, List<Room*>* room);
	void SetScale(float scale);
	void SetAlpha(int alpha);
	void MiniMapPrint(iPoint pos);

private:
	
	Application* app = nullptr;
	bool isHub = true;
	SDL_Texture* textureHub = nullptr;
	float scale = 1.0f;
	int alpha = 255;
	List<Room*> rooms;
};

#endif //__MINIMAP_H__