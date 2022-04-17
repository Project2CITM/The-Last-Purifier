#pragma once
#include "GameObject.h"
#include "Text.h"


class Sensor;
class Trigger;

class IntLabel : public GameObject
{
public:
	IntLabel(std::string sentence, std::string name, iPoint position, int radius = 40);
	~IntLabel();

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;
	void OnTriggerExit(std::string trigger, PhysBody* col) override;

private:

	std::string sentence;

	bool canSpeak = true;
	bool nearNpc = false;
	bool speaking = false;

	iPoint textPosition = { 0,0 };

	iPoint playerPos = { 0,0 };

	Text* text = nullptr;

	Trigger* trigger = nullptr;

private:
	void pixelCalculator();
};
