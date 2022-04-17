#include "Instructor.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "Trigger.h"
#include "Text.h"

Instructor::Instructor(std::string sentence, std::string name, iPoint position) : NPC(name, position)
{
	this->sentence = sentence;
	this->name = name;
	this->position = position;

	textPosition = { position.x - npcData.w, position.y - npcData.h };
	//trigger = new Trigger({ position.x + 10,position.y + 10 }, 40, this, "triggerInstructor", false);

	InitRenderObjectWithXml("revenantInstructor");

	for (int i = 0; i < 2; i++)
	{
		idleAnim.PushBack({ 70 * i,0,70,95 });
	}
	idleAnim.loop = true;
	idleAnim.speed = 0.05;
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

	classTreeHud = new ClassTreeHud();

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
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || 
			app->input->GetControllerButton(BUTTON_A) == KEY_DOWN)
		{
			toggle = !toggle;
		}
	}

	if(toggle) classTreeHud->Update();
	
}

void Instructor::PostUpdate()
{

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
	}

}