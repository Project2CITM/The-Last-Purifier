#include "ButtonObject.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"

ButtonObject::ButtonObject(iPoint position, int color) : GameObject("ButtonObject", "ButtonObject")
{
	buttonColor = color;

	// Init RenderObject
	renderObjects[0].InitAsTexture(app->textures->Load("Assets/Sprites/Puzzles/buttons152x120.png"), position, buttonSectionsUp[buttonColor], 0.15f, 0, 0);

	renderObjects[0].textureCenterX = 76;
	renderObjects[0].textureCenterY = 60;

	// Init physbody

	pBody = app->physics->CreateRectangleSensor(position, 8, 8, this);

	b2Filter filter;
	filter.categoryBits = app->physics->TRIGGER_LAYER;
	filter.maskBits = app->physics->EVERY_LAYER & ~app->physics->ENEMY_LAYER & ~app->physics->TRIGGER_LAYER;
	pBody->body->GetFixtureList()->SetFilterData(filter);
}

void ButtonObject::PreUpdate()
{
}

void ButtonObject::PostUpdate()
{
	if (isPressed) renderObjects->section = buttonSectionsDown[buttonColor];
	else  renderObjects->section = buttonSectionsUp[buttonColor];
	GameObject::PostUpdate();
}

void ButtonObject::CleanUp()
{
}

void ButtonObject::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->name == "Player")
	{
		PressButton();
	}
}

void ButtonObject::PressButton()
{
	isPressed = true;
}

void ButtonObject::ResetButton()
{
	isPressed = false;
}
