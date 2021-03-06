#include "Instructor.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "Trigger.h"
#include "Text.h"
#include "ModuleMap.h"

Instructor::Instructor(std::string sentence, std::string name, iPoint position, PlayerClass pClass) : NPC(name, position)
{
	this->sentence = sentence;
	this->name = name;
	this->position = position;
	this->pClass = pClass;

	textPosition = { position.x - npcData.w, position.y - npcData.h };
	//trigger = new Trigger({ position.x + 10,position.y + 10 }, 40, this, "triggerInstructor", false);

	switch (pClass)
	{
	case PlayerClass::REVENANT:
		InitRenderObjectWithXml("revenantInstructor");
		break;
	case PlayerClass::SAGE:
		InitRenderObjectWithXml("sageInstructor");
		break;
	}

	for (int i = 0; i < 2; i++)
	{
		idleAnim.PushBack({ 70 * i,0,70,95 });
	}
	idleAnim.loop = true;
	idleAnim.duration = 0.320;
	idleAnim.hasIdle = false;

	Instructor::Start();
}

Instructor::~Instructor()
{

}

void Instructor::Start()
{
	text = new Text(textPosition, " ");
	text->ChangeDrawMode();

	classTreeHud = new ClassTreeHud(pClass);

}

void Instructor::CleanUp()
{
	if (classTreeHud != nullptr)
	{
		classTreeHud->CleanUp();
		RELEASE(classTreeHud);
	}
}

void Instructor::PreUpdate()
{
	if (toggle) classTreeHud->PreUpdate();
}

void Instructor::Update()
{
	if (canSpeak && nearNpc)
	{
		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			toggle = !toggle;
			app->ToggleMenu(!app->isMenu);

			if (toggle) app->events->TriggerEvent(GameEvent::STOP_PLAYER_MOVEMENT);
			else app->events->TriggerEvent(GameEvent::RESUME_PLAYER_MOVEMENT);

			app->events->TriggerEvent(GameEvent::SAVE_GAME);

			classTreeHud->TextCleaning();
		}
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			toggle = false;
			app->ToggleMenu(false);
			app->events->TriggerEvent(GameEvent::RESUME_PLAYER_MOVEMENT);

			classTreeHud->TextCleaning();

			app->events->TriggerEvent(GameEvent::SAVE_GAME);
		}
		if (app->input->GetControllerButton(BUTTON_A) == KEY_DOWN && !toggle)
		{
			toggle = true;
			app->ToggleMenu(true);
			app->events->TriggerEvent(GameEvent::STOP_PLAYER_MOVEMENT);
			classTreeHud->isFirstFrame = true;

			classTreeHud->TextCleaning();

			app->events->TriggerEvent(GameEvent::SAVE_GAME);
		}
		if (app->input->GetControllerButton(BUTTON_B) == KEY_DOWN && toggle)
		{
			toggle = false;
			app->ToggleMenu(false);
			app->events->TriggerEvent(GameEvent::RESUME_PLAYER_MOVEMENT);

			classTreeHud->TextCleaning();

			app->events->TriggerEvent(GameEvent::SAVE_GAME);
		}
	}

	if(toggle) classTreeHud->Update();
	
}

void Instructor::PostUpdate()
{
	if (exterior && !app->map->roof) return;
	if (!exterior && app->map->roof) return;

	if (toggle) classTreeHud->PostUpdate();

	idleAnim.Update();
	renderObjects[0].section = idleAnim.GetCurrentFrame();
	GameObject::PostUpdate();
}

void Instructor::OnTriggerEnter(std::string trigger, PhysBody* col) {

	if (col->gameObject->name == "Player")
	{
		nearNpc = true;

		if (!speaking)
		{
			text->SetText(sentence);
		}
	}
}
void Instructor::OnTriggerExit(std::string trigger, PhysBody* col) {

	if (col->gameObject->name == "Player") {
		nearNpc = false;
		speaking = false;
		text->SetText(" ");
		sentenceOrder = 0;
		toggle = false;

		//PREVENTS A WEIRD BUG
		app->events->TriggerEvent(GameEvent::RESUME_PLAYER_MOVEMENT);
		classTreeHud->TextCleaning();
	}

}