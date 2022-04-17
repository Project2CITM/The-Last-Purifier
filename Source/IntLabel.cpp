#include "IntLabel.h"
#include "ModulePhysics.h"
#include "Trigger.h"
#include "Text.h"

#include <iostream>

IntLabel::IntLabel(std::string sentence, std::string name, iPoint position, int radius) : GameObject (name, "Label")
{
	this->sentence = sentence;
	this->name = name;
	this->position = position;


	textPosition = { 255 , 20 };
	trigger = new Trigger({ position.x + 10,position.y + 10 }, radius, this, "triggerLabel", false);

	Start();
}

IntLabel::~IntLabel()
{

}

void IntLabel::Start()
{
	text = new Text(textPosition, " ");
	//text->SetColor({ 197 ,20 , 0, 255 });
	//text->ChangeDrawMode();
}

void IntLabel::PreUpdate()
{

}

void IntLabel::Update()
{

}

void IntLabel::PostUpdate()
{

	GameObject::PostUpdate();
}

void IntLabel::CleanUp()
{

}

void IntLabel::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->name == "Player")
	{
		nearNpc = true;

		if (!speaking)
		{
			text->SetText(sentence);
		}
	}
}

void IntLabel::OnTriggerExit(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->name == "Player") {
		nearNpc = false;
		speaking = false;
		text->SetText(" ");
	}
}