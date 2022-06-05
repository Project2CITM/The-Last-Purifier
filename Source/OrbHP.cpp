#include "OrbHP.h"
#include "Player.h"

OrbHP::OrbHP():GameObject("OrbHP","OrbHP")
{

}

OrbHP::~OrbHP()
{

}

void OrbHP::Start()
{

}

void OrbHP::PreUpdate()
{

}

void OrbHP::Update()
{

}

void OrbHP::PostUpdate()
{

}

void OrbHP::CleanUp()
{
	//app->events->RemoveListener(this);
}

void OrbHP::OnTriggerEnter(std::string trigger, PhysBody* col)
{
	//PlusHP();
	//pendingToDelete = true;
}

void OrbHP::PlusHP()
{
	//player->hpPlayer += 20;
}
