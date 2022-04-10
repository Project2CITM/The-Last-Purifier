#include "MapLoader.h"

void MapLoader::ExtractMapColliders(Room* r)
{
	LoadMap(r->id);
	CreateColliders(r);
}

void MapLoader::LoadMap(short id)
{
	//Create path from id
	string filePath = "Assets/Maps/map";
	if (id > 0) {
		filePath += to_string(id);
	}
	else {
		switch (id) {
		case -1:
			filePath += "MiniBoss";	//CHANGE boss room
			break;
		case -2:
			filePath += "MiniBoss";
			break;
		case -3:
			filePath += "Start";
			break;
		}
	}
	filePath += ".tmx";

	//Pick xml from path
	pugi::xml_parse_result result = mapDoc.load_file(filePath.c_str());
	if (result == NULL)
		LOG("Could not load (map) xml file: %s. pugi error: %s", filePath, result.description())
	else
		mapNode = mapDoc.child("map");
}

void MapLoader::CreateColliders(Room* r)
{
	//Find collision layer
	pugi::xml_node colliderLayer = mapNode.child("layer");
	for (colliderLayer; colliderLayer != NULL; colliderLayer = colliderLayer.next_sibling()) {
		string name = colliderLayer.attribute("name").as_string();
		if ( name == "Colisions") break;
	}

	//Get node tiles (with gid attributes)
	pugi::xml_node tile = colliderLayer.first_child().first_child();
	for (int i = 0; i < MAX_ROOM_TILES_ROWS; ++i) {
		for (int j = 0; j < MAX_ROOM_TILES_COLUMNS; ++j) {
			//if there is gid, create collider
			if (tile.first_attribute().as_int() == 2561) {
				r->colliders.add(app->physics->CreateRectangle(iPoint(
					r->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE + j * TILE_SIZE + TILE_SIZE / 2,
					r->roomPosition.y * MAX_ROOM_TILES_ROWS * TILE_SIZE + i * TILE_SIZE + TILE_SIZE / 2),
					TILE_SIZE / 2, TILE_SIZE / 2, nullptr, b2_staticBody));
			}
			tile = tile.next_sibling();
		}
	}
}