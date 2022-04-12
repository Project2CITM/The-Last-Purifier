#pragma once
#include "Scene.h"
#include "GUIButton.h"
#include "ClassTree.h"

class ClassTreeHud : public Scene
{
public:

	ClassTreeHud();
	~ClassTreeHud();

	virtual bool InitScene();
	bool Start() override;
	bool CleanUp() override;

	bool PreUpdate() override;
	bool Update() override;
	bool PostUpdate() override;
	

private:

	bool active = false;
	int btnSize = 40;

	SDL_Texture* treeTexture = nullptr;

	SDL_Rect bRect = { 0, 0, 0, 0 };

	iPoint bPoint = { 0, 0 };
	iPoint testBtnPoint = { 0, 0 };

    //GUIButton* testBtn = nullptr;
	//GUIButton* unlockBtn[TREE_SIZE];

};

