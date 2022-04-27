#include "Quest.h"
#include "NPC.h"

Quest::Quest(iPoint position, std::string questNum, std::string name):GameObject("Quest", "Quest")
{
	NPC* npc = new NPC(name, this->position);


}

Quest::~Quest()
{
}

void Quest::Start()
{
}

void Quest::PreUpdate()
{
}

void Quest::Update()
{
}

void Quest::PostUpdate()
{
}

void Quest::CleanUp()
{
}
