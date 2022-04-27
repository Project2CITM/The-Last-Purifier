#include "MiniMap.h"
#include "RoomManager.h"

void MiniMap::Init(bool hub)
{
	if (hub) {
		hubState = true;
		//TODO: change minimap hub png or smth
	}
	else {
		hubState = false;
		//Charge room manager rooms for rects
	}
}

void MiniMap::Update()
{
	//Update minimap every frame in hub, when changing room ingame
}

void MiniMap::SetScale(float scale)
{
	if (scale < 0) return;
	this->scale = scale;
}

void MiniMap::MiniMapPrint(iPoint pos)
{
	//Scale 0 - No map
	if (scale == 0.0f) return;

	if (hubState) {
		//print for hub

		return;
	}

	//print ingame scene


}
