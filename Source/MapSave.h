#ifndef __MAPSAVE_H__
#define __MAPSAVE_H__

#include "GameObject.h"
#include "List.h"
#include <time.h>

#define MAPSAVE_NAME "SaveMap.xml"

class Room;

class MapSave
{
public:
	MapSave() {
		this->app = Application::GetInstance();
	}

	~MapSave() {}

	//Open XML document
	void Init();
	
	//Returns current seed, 0 if no seed.
	int CurrentSeed();
	
	//Returns new seed.
	int GenerateSeed();
	
	//Implement the saved seed
	void UseCurrentSeed();
	
	//Use your own seed
	void UseSeed(int seed);
	
	//Erase seed
	void ClearSeed();

	//Save done and undone rooms
	void SaveRoomStates(List<Room*>* rooms);
	
	//Erase room states
	void ClearRoomStates();

	//Use the saved room states
	void UseRoomStates(List<Room*>* rooms);


private:
	Application* app;
	pugi::xml_document mapDoc;
	pugi::xml_node mapNode;
};

#endif //__MAPSAVE_H__