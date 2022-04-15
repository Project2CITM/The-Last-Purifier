#ifndef __NPC_H__
#define __NPC_H__

#include "GameObject.h"

class Text;

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

public:
	List<std::string> sentences;

	bool canSpeak = true;

	//iPoint npcPosition = { 0,0 };  // Ya existe position en GameObject, puedes utilizar funcion GetPosition() para obtenerlo
	iPoint textPosition={0,0};

	struct DATA 
	{
		int w = 32;
		int h = 64;
	}npcData;

private:
	Text* text = nullptr;

	pugi::xml_node configDialog;

	int sentenceOrder = 0;

	SDL_Rect npcRect;
};

#endif // !__NPC_H__