#pragma once
#include "GameObject.h"
class Quest : public GameObject
{
	Quest(iPoint position, std::string questNum, std::string name);

	~Quest();

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;


};

