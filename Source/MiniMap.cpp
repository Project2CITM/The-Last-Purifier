#include "MiniMap.h"
#include "RoomManager.h"

MiniMap::~MiniMap()
{
	app = nullptr;
	isHub = true;
	textureHub = nullptr;
	scale = 1.0f;
	alpha = 255;
	rooms.clearPtr();
}

void MiniMap::Init(bool isHub, List<Room*> rooms)
{
	if (isHub) {
		this->isHub = true;
		//TODO: change minimap hub png or smth
	}
	else {
		this->isHub = false;
		this->rooms = rooms;
	}
}

void MiniMap::SetScale(float scale)
{
	if (scale < 0.0f) return;
	this->scale = scale;
}

void MiniMap::SetAlpha(int alpha)
{
	if (alpha < 0 || alpha > 255) return;
	this->alpha = alpha;
}

void MiniMap::MiniMapPrint(iPoint pos)
{
	//Scale 0 - No map
	if (scale == 0.0f) return;

	//Minimap Frame
	app->renderer->AddRectRenderQueue({ pos.x, pos.y, int (DEFAULT_WIDTH * scale), int (DEFAULT_HEIGHT * scale) }, { 200, 200, 200, 255 }, false, 2);

	if (isHub) {
		//TODO: print for hub

		return;
	}

	//TODO: print ingame scene


}
