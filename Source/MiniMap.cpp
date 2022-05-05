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

void MiniMap::SetScale(int scale)
{
	if (scale < 0) return;
	this->scale = scale;
}

void MiniMap::SetAlpha(int alpha)
{
	if (alpha < 0 || alpha > 255) return;
	this->alpha = alpha;
}

void MiniMap::MiniMapPrint(iPoint pos, iPoint playerPos)
{
	//Minimap Frame
	pos.x += DEFAULT_WIDTH;
	pos.y += DEFAULT_HEIGHT;
	pos.x -= DEFAULT_WIDTH * scale;
	pos.y -= DEFAULT_HEIGHT * scale;
	SDL_Rect outRect = { pos.x, pos.y, DEFAULT_WIDTH * scale, DEFAULT_HEIGHT * scale };

	//Print Hub
	if (isHub) {
		//TODO: print for hub

		//Print minimap frame
		app->renderer->AddRectRenderQueue(outRect, { 200, 200, 200, 255 }, false, 3, 2.0f, 0.0f);
		return;
	}

	//Print ingame scene
	for (int i = 0; i < rooms.count(); i++) {
		
		SDL_Color color{ 100, 100, 100 , alpha};
		
		switch (rooms[i]->id) {
		case -1:
			color.r += 100;
			color.b -= 100;
			color.g -= 100;
			break;
		case -2:
			color.r += 55;
			color.g -= 100;
			break;
		case -3:	
			color.b -= 100;
			color.r -= 100;
			break;
		}
		
		if (rooms[i]->done) {
			color.r += 100;
			color.g += 100;
			color.b += 100;
		}
		
		iPoint position = rooms[i]->roomPosition;

		position += {2*scale, 2*scale};
		position -= playerPos;

		position.x *= RECT_WIDTH;
		position.y *= RECT_HEIGHT;
		position += pos;
		
		SDL_Rect rect{ position.x, position.y, RECT_WIDTH, RECT_HEIGHT };
		
		if (SDL_HasIntersection(&outRect, &rect)) {
			/*
			SDL_Rect resRect;
			SDL_UnionRect(&outRect, &rect, &resRect);
			if (!SDL_RectEquals(&resRect, &outRect)) {
				rect.w /= 2;
				rect.h /= 2;
			}
			*/
			rect.x += 1;
			rect.y += 1;
			rect.w -= 2;
			rect.h -= 2;
			app->renderer->AddRectRenderQueue(rect, color, true, 3, 2.0f, 0.0f);
		}
	}
	//Print minimap frame
	app->renderer->AddRectRenderQueue(outRect, { 200, 200, 200, 255 }, false, 3, 2.0f, 0.0f);
}
