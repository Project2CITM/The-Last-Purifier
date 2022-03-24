#include "Room.h"

void Room::CloseDoors()
{
	ListItem<Door*>* currentDoor = doors.start;
	while (currentDoor != nullptr) {
		if (currentDoor->data->open) {
			currentDoor->data->open = false;
			currentDoor->data->collider = Application::GetInstance()->physics->CreateRectangle(currentDoor->data->pos, TILE_SIZE, TILE_SIZE);
		}
		currentDoor = currentDoor->next;
	}
}

void Room::OpenDoors()
{
	ListItem<Door*>* currentDoor = doors.start;
	while (currentDoor != nullptr) {
		if (!currentDoor->data->open) {
			currentDoor->data->open = true;
			RELEASE(currentDoor->data->collider);
		}
		currentDoor = currentDoor->next;
	}
}

void Room::CleanUp()
{
	ListItem<Door*>* currentDoor = doors.start;
	while (currentDoor != nullptr) {
		currentDoor->data->open = true;
		RELEASE(currentDoor->data->collider);
		currentDoor = currentDoor->next;
	}
	doors.clearPtr();
}
