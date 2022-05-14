#pragma once
#include "GameObject.h"
class ButtonPuzzle : public GameObject
{
public:
	ButtonPuzzle();

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void CleanUp();
};

