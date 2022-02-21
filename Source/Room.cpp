#include "Room.h"

void Room::CloseDoors(Application* app)
{
	ListItem<Door*>* currentDoor = doors.start;
	while (currentDoor != nullptr) {
		currentDoor->data->open = false;
		currentDoor->data->collider = app->physics->CreateRectangle(currentDoor->data->pos, TILE_SIZE, TILE_SIZE);
		currentDoor = currentDoor->next;
	}
}

void Room::OpenDoors(Application* app)
{
	ListItem<Door*>* currentDoor = doors.start;
	while (currentDoor != nullptr) {
		currentDoor->data->open = true;
		RELEASE(currentDoor->data->collider);
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
