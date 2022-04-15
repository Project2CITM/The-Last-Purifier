#include "NPC.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Text.h"
#include "ModuleRender.h"

NPC::NPC(std::string name, iPoint position) : GameObject(name,"NPC")
{
	this->position = position;
	textPosition = { position.x, position.y - npcData.h };
	npcRect = { position.x, position.y, npcData.w,npcData.h };
}

NPC::~NPC()
{

}

void NPC::Start()
{
	text = new Text({textPosition},"Pulse enter para hablar");
	configDialog = app->config.child("dialogText");

	pugi::xml_node npcNode = configDialog.child(name.c_str());
	int sentenceNum = npcNode.attribute("Num").as_int(0);

	for (int i = 1; i <= sentenceNum; i++)
	{
	 	std::string temporalSentence = "Sentence" + std::to_string(i);
		
		sentences.add(npcNode.child(temporalSentence.c_str()).child_value());
	}
	//Aqui se imprime el sprite del NPC
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
	
	app->renderer->AddRectRenderQueue(npcRect, SDL_Color{ 250,0,0,255 }, true, 3, 50);
}

void NPC::CleanUp()
{
	sentences.clear();
	//RELEASE(text); //No funciona no quitar barras
}
