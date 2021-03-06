#include "NPC.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Text.h"
#include "SceneGame.h"
#include "ModuleRender.h"
#include "PlayerController.h"
#include "SceneGame.h"
#include "ModulePhysics.h"
#include "ModuleMap.h"
#include "ModuleAudio.h"

NPC::NPC(std::string name, iPoint position) : GameObject(name,"NPC")
{
	this->position = position;

	textPosition = { position.x-npcData.w, position.y - npcData.h };

	trigger = new Trigger({position.x+10,position.y+10}, 50, this,"triggerNpc",false);

	b2Filter filter;

	filter.categoryBits = app->physics->TRIGGER_LAYER;

	trigger->pBody->body->GetFixtureList()->SetFilterData(filter);

	InitRenderObjectWithXml("npc");

	for (int i = 0; i < 2; i++)
	{
		idleAnim.PushBack({70*i,0,70,95});
	}

	idleAnim.loop = true;

	idleAnim.duration = 0.320;

	idleAnim.hasIdle = false;	
	
	dialogNPCFX[0] = app->audio->LoadFx("Audio/SFX/NPCs/sfx_npcPhrase1.wav");
	dialogNPCFX[1] = app->audio->LoadFx("Audio/SFX/NPCs/sfx_npcPhrase2.wav");
	dialogNPCFX[2] = app->audio->LoadFx("Audio/SFX/NPCs/sfx_npcPhrase3.wav");
	dialogNPCFX[3] = app->audio->LoadFx("Audio/SFX/NPCs/sfx_npcPhrase4.wav");
}

NPC::~NPC()
{

}

void NPC::Start()
{
	text = new Text(textPosition," ");
	text->ChangeDrawMode();
	configDialog = app->config.child("dialogText");

	pugi::xml_node npcNode = configDialog.child("english").child(name.c_str());
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
	if (nearNpc)
	{
		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN || app->input->GetControllerButton(BUTTON_A) == KEY_DOWN)
		{
			speak = true;

			int num = rand() % 4;

			if (!speaking) {
				speaking = true;
				app->audio->PlayFx(dialogNPCFX[num]);
			}
			if (sentenceOrder >= sentences.count())
			{
				text->SetText("");
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
	if (exterior && !app->map->roof) return;
	if (!exterior && app->map->roof) return;
	

	idleAnim.Update();
	renderObjects[0].section = idleAnim.GetCurrentFrame();
	GameObject::PostUpdate();
}

void NPC::CleanUp()
{
	sentences.clear();
	//RELEASE(text); //No funciona no quitar barras
}

void NPC::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->name == "Player") 
	{
		nearNpc = true;

		LOG("Enter");
		if (!speaking) 
		{
			
			if (app->input->usingGameController)text->SetText("               Press A to talk");
			else text->SetText("               Press F to talk");
		}	
	}
}

void NPC::OnTriggerExit(std::string trigger, PhysBody* col) 
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->name == "Player") 
	{
		LOG("Exit");
		nearNpc = false;
		speaking = false;
		text->SetText(" ");
		sentenceOrder = 0;
	}

}