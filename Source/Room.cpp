#include "Room.h"

void Room::CloseDoors()
{
	/*ListItem<Door*>* currentDoor = doors.start;
	while (currentDoor != nullptr) {
		if (currentDoor->data->open) {
			currentDoor->data->open = false;
			currentDoor->data->collider = Application::GetInstance()->physics->CreateStatic(
				iPoint(currentDoor->data->pos.x - (TILE_SIZE * currentDoor->data->size.x / 2), currentDoor->data->pos.y - (TILE_SIZE * currentDoor->data->size.y / 2)),
				(TILE_SIZE * currentDoor->data->size.x) / 2, (TILE_SIZE * currentDoor->data->size.y) / 2);
		}
		currentDoor = currentDoor->next;
	}*/
}

void Room::OpenDoors()
{
	ListItem<Door*>* currentDoor = doors.start;
	while (currentDoor != nullptr) {
		if (!currentDoor->data->open) {
			currentDoor->data->open = true;
			currentDoor->data->collider->GetWorld()->DestroyBody(currentDoor->data->collider);
		}
		currentDoor = currentDoor->next;
	}
}

void Room::DrawRoom()
{
	app->renderer->AddTextureRenderQueue(roomTexture, 
		iPoint(roomPosition.x * MAX_ROOM_TILES_COLUMNS, roomPosition.y * MAX_ROOM_TILES_ROWS) * TILE_SIZE, 
		{0,0,0,0}, TILE_SIZE / 16.0f);
}

void Room::CleanUp()
{
	ListItem<Door*>* currentDoor = doors.start;
	while (currentDoor != nullptr) {
		currentDoor->data->open = true;
		currentDoor->data->collider->GetWorld()->DestroyBody(currentDoor->data->collider);
		currentDoor = currentDoor->next;
	}
	doors.clearPtr();

	//delete roomTexture;
}
