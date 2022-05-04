#include "MiniMap.h"

MiniMap::~MiniMap()
{
	app = nullptr;
	isHub = true;
	textureHub = nullptr;
	scale = 1.0f;
	alpha = 255;
	rooms.clearPtr();
}

void MiniMap::Init(bool isHub, List<Room*>* room)
{
	if (isHub) {
		this->isHub = true;
		//TODO: change minimap hub png or smth
	}
	else {
		this->isHub = false;
		this->rooms = *room;
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
	app->renderer->AddRectRenderQueue({ pos.x, pos.y, int (DEFAULT_WIDTH * scale), int (DEFAULT_HEIGHT * scale) }, { 200, 200, 200, 255 }, false, 3, 2.0f, 0.0f);

	if (isHub) {
		//TODO: print for hub

		return;
	}

	//TODO: print ingame scene
	for (int i = 0; i < rooms.count(); i++) {
		
		SDL_Color color{ 100, 100, 100 , alpha};
		if (rooms[i]->done) {
			color.r = 200;
			color.g = 200;
			color.b = 200;
		}
		
		iPoint position = rooms[i]->roomPosition;

		position -= {2, 1};

		position.x *= RECT_WIDTH;
		position.y *= RECT_HEIGHT;
		position += pos;

		SDL_Rect rect{ position.x, position.y, RECT_WIDTH * scale, RECT_HEIGHT * scale };
		app->renderer->AddRectRenderQueue(rect, color, true, 3, 2.0f, 0.0f);
		//app->renderer->AddRectRenderQueue(rect, SDL_Color{ 0,0,0 }, false, 3, 1.9f, 0.0f);
	}

}
