#pragma once
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


private:

	Text* advisor=nullptr;

	pugi::xml_node configDialog;

	int sentenceOrder = 0;
};

