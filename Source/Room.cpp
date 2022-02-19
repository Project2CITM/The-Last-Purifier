#include "Room.h"

void Room::CreateDoors()
{

}

void Room::CloseDoors()
{
	ListItem<Door*>* currentDoor = doors.start;
	while (currentDoor != nullptr) {
		currentDoor->data->open = false;
		currentDoor = currentDoor->next;
	}
}

void Room::OpenDoors()
{
	ListItem<Door*>* currentDoor = doors.start;
	while (currentDoor != nullptr) {
		currentDoor->data->open = true;
		currentDoor = currentDoor->next;
	}
}
