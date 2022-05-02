#ifndef __NPC_H__
#define __NPC_H__

#include "GameObject.h"
#include "Animation.h"

class Text;
class Trigger;
class NPC : public GameObject
{
public:
	NPC(std::string name,iPoint position);

	~NPC();

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;

	void OnTriggerExit(std::string trigger, PhysBody* col) override;


public:
	List<std::string> sentences;

	bool canSpeak = true;

	bool speaking = false;

	//iPoint npcPosition = { 0,0 };  // Ya existe position en GameObject, puedes utilizar funcion GetPosition() para obtenerlo
	iPoint textPosition={0,0};

	struct DATA 
	{
		int w = 170;
		int h = 32;
	}npcData;

	iPoint playerPos = { 0,0 };

	bool questActive = false;

protected:
	Text* text = nullptr;

	pugi::xml_node configDialog;

	int sentenceOrder = 0;

	Trigger* trigger = nullptr;

	Animation idleAnim;

	bool nearNpc = false;

	bool exterior = true;

public:
	virtual void setIndoors() { exterior = false; }
	virtual void setOutDoors() { exterior = true; }
private:
	uint dialogNPCFX[4];
};

#endif // !__NPC_H__