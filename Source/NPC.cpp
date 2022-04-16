#include "NPC.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Text.h"
#include "SceneGame.h"
#include "ModuleRender.h"
#include "PlayerController.h"
#include "SceneGame.h"
#include "Trigger.h"
#include "ModulePhysics.h"

NPC::NPC(std::string name, iPoint position) : GameObject(name,"NPC")
{
	this->position = position;
	textPosition = { position.x, position.y - npcData.h };
	npcSensor = { position.x - 32,position.y - 64,96,192 };
	trigger = new Trigger({position.x+10,position.y+10}, 50, this,"triggerNpc",false);

	b2Filter filter;
	filter.categoryBits = app->physics->TRIGGER_LAYER;
	trigger->pBody->body->GetFixtureList()->SetFilterData(filter);

	InitRenderObjectWithXml("npc");

	for (int i = 0; i < 2; i++)
	{
		idleAnim.PushBack({81*i,0,81,111});
	}
	idleAnim.loop = true;
	idleAnim.speed = 0.2;
	idleAnim.hasIdle = false;
}

NPC::~NPC()
{

}

void NPC::Start()
{
	text = new Text({255,150}," ");
	configDialog = app->config.child("dialogText");

	pugi::xml_node npcNode = configDialog.child(name.c_str());
	int sentenceNum = npcNode.attribute("Num").as_int(0);

	for (int i = 1; i <= sentenceNum; i++)
	{
	 	std::string temporalSentence = "Sentence" + std::to_string(i);
		
		sentences.add(npcNode.child(temporalSentence.c_str()).child_value());
	}

}

void NPC::PreUpdate()
{

}

void NPC::Update()
{
//	npcRect = { npcPosition.x,npcPosition.y,npcData.w,npcData.h };
	if (canSpeak) 
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) 
		{
			if (!speaking) {
				speaking = true;
			}
			if (sentenceOrder >= sentences.count())
			{
				text->SetText(" ");		
			}
			else 
			{
				text->SetText(sentences[sentenceOrder]);
				sentenceOrder++;
			}
		}
	}
}

void NPC::PostUpdate()
{
	idleAnim.Update();
	renderObjects[0].section = idleAnim.GetCurrentFrame();
	GameObject::PostUpdate();
}

void NPC::CleanUp()
{
	sentences.clear();
	//RELEASE(text); //No funciona no quitar barras
}

void NPC::OnTriggerEnter(std::string trigger, PhysBody* col) {

	if (col->gameObject->name == "Player") {
		if (!speaking) {
			text->SetText("Pulse enter para hablar");
		}
		
	}
}