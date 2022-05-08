#include "MapSave.h"
#include "Room.h"

void MapSave::Init()
{
	//Pick xml
	pugi::xml_parse_result result = mapDoc.load_file("SaveMap.xml");
	if (result == NULL)
		LOG("Could not load (SaveMap) xml file. Pugi error: %s", result.description())
	else
		mapNode = mapDoc.child("map");
}

int MapSave::CurrentSeed()
{
	return mapNode.attribute("seed").as_int();
}

int MapSave::GenerateSeed()
{
	int seed = time(NULL);
	srand(seed);
	mapNode.attribute("seed").set_value(seed);
	mapDoc.save_file(MAPSAVE_NAME);
	return seed;
}

void MapSave::UseCurrentSeed()
{
	srand(mapNode.attribute("seed").as_int());
}

void MapSave::UseSeed(int seed)
{
	srand(seed);
	mapNode.attribute("seed").set_value(seed);
	mapDoc.save_file(MAPSAVE_NAME);
}

void MapSave::ClearSeed()
{
	mapNode.attribute("seed").set_value(0);
	mapDoc.save_file(MAPSAVE_NAME);
}

void MapSave::SaveRoomStates(List<Room*>* rooms)
{
	ClearRoomStates();
	for (int i = 0; i < rooms->count(); i++) {
		mapNode.append_child("room").append_attribute("done").set_value(rooms->At(i)->data->done);
	}
	mapDoc.save_file(MAPSAVE_NAME);
}

void MapSave::ClearRoomStates()
{
	while (!mapNode.first_child().empty()) {
		mapNode.remove_child(mapNode.first_child().name());
	}
	mapDoc.save_file(MAPSAVE_NAME);
}

void MapSave::UseRoomStates(List<Room*>* rooms)
{
	pugi::xml_node roomNode = mapNode.first_child();
	for (int i = 0; i < rooms->count(); i++) {
		rooms->At(i)->data->done = roomNode.first_attribute().as_bool();
		if (rooms->At(i)->data->done) {
			rooms->At(i)->data->ClearEnemies();
		}
		roomNode = roomNode.next_sibling();
	}
}