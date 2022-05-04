#include "Quest.h"

Quest::Quest(iPoint position, std::string questNum, std::string name):GameObject("Quest", "Quest")
{
	this->position = position;
	textPosition = { position.x - npcData.w, position.y - npcData.h };
	trigger = new Trigger({ position.x + 10,position.y + 10 }, 50, this, "triggerNpc", false);

	b2Filter filter;
	filter.categoryBits = app->physics->TRIGGER_LAYER;
	trigger->pBody->body->GetFixtureList()->SetFilterData(filter);

	InitRenderObjectWithXml("npc");

	for (int i = 0; i < 2; i++)
	{
		idleAnim.PushBack({ 70 * i,0,70,95 });
	}
	idleAnim.loop = true;
	idleAnim.duration = 0.0320;
	idleAnim.hasIdle = false;


	dialogNPCFX[0] = app->audio->LoadFx("Assets/Audio/SFX/NPCs/sfx_npcPhrase1.wav");
	dialogNPCFX[1] = app->audio->LoadFx("Assets/Audio/SFX/NPCs/sfx_npcPhrase2.wav");
	dialogNPCFX[2] = app->audio->LoadFx("Assets/Audio/SFX/NPCs/sfx_npcPhrase3.wav");
	dialogNPCFX[3] = app->audio->LoadFx("Assets/Audio/SFX/NPCs/sfx_npcPhrase4.wav");


}

Quest::~Quest()
{
}

void Quest::Start()
{

	text = new Text(textPosition, " ");
	text->ChangeDrawMode();
	configDialog = app->config.child("quest");
	
	pugi::xml_node npcNode = configDialog.child("english").child(name.c_str());
	int sentenceNum = npcNode.attribute("Num").as_int(0);
	pugi::xml_node defaultNode = npcNode.child("default");
	pugi::xml_node doneNode = npcNode.child("done");

	if (active) {
		for (int i = 1; i <= sentenceNum; i++)
		{
			std::string temporalSentence = "Sentence" + std::to_string(i);

			sentences.add(defaultNode.child(temporalSentence.c_str()).child_value());
		}
	}
	else {
		for (int i = 1; i <= sentenceNum; i++)
		{
			std::string temporalSentence = "Sentence" + std::to_string(i);

			sentences.add(doneNode.child(temporalSentence.c_str()).child_value());
		}
	}

}

void Quest::PreUpdate()
{
}

void Quest::Update()
{
	if (nearNpc) {
		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN || app->input->GetControllerButton(BUTTON_A) == KEY_DOWN)
		{
			int num = rand() % (4);

			if (!speaking) {
				speaking = true;
				app->audio->PlayFx(dialogNPCFX[num]);
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

void Quest::PostUpdate()
{
	if (finished) pendingToDelete = true;

	if (exterior && !app->map->roof) return;
	if (!exterior && app->map->roof) return;


	idleAnim.Update();
	renderObjects[0].section = idleAnim.GetCurrentFrame();
	GameObject::PostUpdate();
}

void Quest::CleanUp()
{
}

void Quest::OnTriggerEnter(std::string trigger, PhysBody* col)
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

void Quest::OnTriggerExit(std::string trigger, PhysBody* col)
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
