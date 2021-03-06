#include "MiniMap.h"

MiniMap::~MiniMap()
{
	app = nullptr;
	isHub = true;
	textureHub = nullptr;
	scale = 1.0f;
	alpha = 255;
}

void MiniMap::Init(bool isHub, List<Room*>* room)
{
	this->icon = app->textures->Load("Sprites/Minimap/miniMapIcon.png");
	
	if (isHub) {
		this->isHub = true;
		this->textureHub = app->textures->Load("Sprites/Minimap/Hub.png");
	}
	else {
		this->isHub = false;
		this->rooms = room;
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
	//pos.y -= DEFAULT_HEIGHT * scale;
	SDL_Rect outRect = { pos.x, pos.y, DEFAULT_WIDTH * scale, DEFAULT_HEIGHT * scale };
	
	//Icon position
	iPoint iconPos = pos;
	iconPos.x += (DEFAULT_WIDTH * scale) / 2 - (8 * scale);
	iconPos.y += (DEFAULT_HEIGHT * scale) / 2 - (8 * scale);
	
	//Hardcoding, im sorry
	if (scale == 2) {
		if (!isHub) {
			iconPos.x -= 7;
			iconPos.y -= 2;
		}
		else {
			iconPos.x += 7;
			iconPos.y += 5;
		}
	}

	//print icon
	app->renderer->AddTextureRenderQueue(icon, iconPos, { 0,0,0,0 }, 1.0f, 3, 3.0f, 0.0f, SDL_FLIP_NONE, 0.0f);
	
	//print rect
	app->renderer->AddRectRenderQueue(outRect, { 200, 200, 200, 255 }, false, 3, 3.0f, 0.0f);

	//Print Hub
	if (isHub) 
	{
		//Print for hub
		playerPos.x /= 10;
		playerPos.y /= 10;
		playerPos += {128, 128};
		SDL_Rect hubRect = { 
			playerPos.x - (DEFAULT_WIDTH * scale / 2), 
			playerPos.y - (DEFAULT_HEIGHT * scale / 2), 
			DEFAULT_WIDTH * scale, 
			DEFAULT_HEIGHT * scale };
		/*
		if (hubRect.x + 256 < outRect.x + (DEFAULT_WIDTH * scale)) {
			hubRect.w += (hubRect.x + 256 - (DEFAULT_WIDTH * scale));
		}
		*/
		app->renderer->AddTextureRenderQueue(textureHub, pos, hubRect, 1.0f, 3, 2.0f, 0.0f, SDL_FLIP_NONE, 0.0f);

		return;
	}

	//Print ingame scene
	for (int i = 0; i < rooms->count(); i++) {
		
		SDL_Color color{ 100, 100, 100 , alpha};
		
		switch (rooms->At(i)->data->id) 
		{
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

		case -4:
		case -5:
		case -6:
			color.b += 155;
			break;
		}
		
		if (rooms->At(i)->data->done) {
			color.r += 100;
			color.g += 100;
			color.b += 100;
		}
		
		iPoint position = rooms->At(i)->data->roomPosition;

		position += {2 * scale, 2 * scale};
		position -= playerPos;

		position.x *= RECT_WIDTH;
		position.y *= RECT_HEIGHT;
		position += pos;
	
		SDL_Rect rect{ position.x, position.y, RECT_WIDTH, RECT_HEIGHT };
		
		if (SDL_HasIntersection(&outRect, &rect)) 
		{
			rect.x += 1;
			rect.y += 1;
			rect.w -= 2;
			rect.h -= 2;
			app->renderer->AddRectRenderQueue(rect, color, true, 3, 2.0f, 0.0f);
		}
	}
}
