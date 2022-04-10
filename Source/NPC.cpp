#include "NPC.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Text.h"

NPC::NPC(std::string name, iPoint position) : GameObject(name,"NPC")
{


}

NPC::~NPC()
{

}

void NPC::Start()
{
	advisor = new Text({5,5},"Pulse enter para hablar");
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
	if (canSpeak) {
		
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			if (sentenceOrder >= sentences.count()) {
				advisor->SetText(" ");
				
			}
			else {

				advisor->SetText(sentences[sentenceOrder]);
				sentenceOrder++;
			}
		}

	}
	

}

void NPC::PostUpdate()
{

}

void NPC::CleanUp()
{

}
