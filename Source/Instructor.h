#ifndef __INSTRUCTOR_H__
#define __INSTRUCTOR_H__

#include "NPC.h"
#include "ClassTreeHud.h"

class Instructor : public NPC
{
public:
	Instructor(std::string sentence, std::string name, iPoint position, PlayerClass pClass);

	~Instructor();

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;

	void OnTriggerExit(std::string trigger, PhysBody* col) override;

public:
	ClassTreeHud* classTreeHud = nullptr;

	PlayerClass pClass = PlayerClass::REVENANT;

	std::string sentence;

private:
	
	bool toggle = false;

	struct DATA
	{
		int w = 35;
		int h = 32;
	}npcData;
};

#endif // !__INSTRUCTOR_H__