#ifndef _TRIGGER_
#define _TRIGGER_
#include "GameObject.h"

// This class use is for game objects that need more than one collider. Add a Trigger to an already existing game object. 
class Trigger : public GameObject
{
public:
	Trigger(iPoint pos, int width, int height, GameObject* father = nullptr, std::string name = "Trigger", bool follow = true);
	Trigger(iPoint pos, int radius, GameObject* father = nullptr, std::string name = "Trigger", bool follow = true);

	void Update();

	void OnCollisionEnter(PhysBody* col) override;
	void OnCollisionExit(PhysBody* col) override;

	iPoint positionOffset = { 0,0 };

private:
	bool followFather = true;
	GameObject* father = nullptr;
};

#endif
