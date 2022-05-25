#ifndef _TRIGGER_
#define _TRIGGER_
#include "GameObject.h"

// This class use is for game objects that need more than one collider. Add a Trigger to an already existing game object. 
class Trigger : public GameObject
{
public:
	Trigger(iPoint pos, int width, int height, GameObject* parent = nullptr, std::string name = "Trigger", bool follow = true);
	Trigger(iPoint pos, int radius, GameObject* parent = nullptr, std::string name = "Trigger", bool follow = true);

	void Update() override;

	void PostUpdate() override;

	void OnCollisionEnter(PhysBody* col) override;

	void OnCollisionExit(PhysBody* col) override;

	void Destroy();

	GameObject* GetParent() { return parent; }

private:

	bool AcceptAction();

public:

	iPoint positionOffset = { 0,0 };

	bool onTriggerEnter = false;

	bool onTriggerStay = false;

	bool onTriggerExit = false;

private:
	bool followFather = true;

	GameObject* parent = nullptr;

	PhysBody* trig = nullptr;
};

#endif

