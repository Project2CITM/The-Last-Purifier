#ifndef __CLASDD_TREE_HUD_H__
#define __CLASDD_TREE_HUD_H__

#include "Scene.h"
#include "GUIButton.h"
#include "ClassTree.h"

class ClassTreeHud : public Scene
{
public:

	ClassTreeHud(PlayerClass pClass = PlayerClass::REVENANT);
	~ClassTreeHud();

	virtual bool InitScene();
	bool Start() override;
	bool CleanUp() override;

	bool PreUpdate() override;
	bool Update() override;
	bool PostUpdate() override;

	//load the texture
	void LoadTexFile(const pugi::xml_document& dataFile);

private:

	int startId = 0;

	bool active = false;
	int btnSize = 40;

	SDL_Texture* treeTexture = nullptr;

	SDL_Rect bRect = { 0, 0, 0, 0 };

	iPoint bPoint = { 0, 0 };
	iPoint testBtnPoint = { 0, 0 };

	PlayerClass pClass;
	ClassTree* tree;
	Player* player;

    //GUIButton* testBtn = nullptr;
	List<GUIButton*>* unlockBtn = nullptr;

	SDL_Texture* tree_Debug = nullptr;
};

#endif // !__CLASDD_TREE_HUD_H__