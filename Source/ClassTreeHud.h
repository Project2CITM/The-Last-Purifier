#ifndef __CLASDD_TREE_HUD_H__
#define __CLASDD_TREE_HUD_H__

#include "Scene.h"
#include "GUIButton.h"
#include "ClassTree.h"
#include "CommonTree.h"
#include "Animation.h"
#include "Text.h"


struct treeFeedback
{
	Animation anim;
	int currentFrame = 0;
	iPoint pos;
	int id = 0;

	treeFeedback(int spellId, int currentFrame, iPoint pos)
	{
		this->currentFrame = currentFrame;
		this->pos = pos;
		

		spellId--;
		for (int i = 0; i < 4; i++)
		{
			anim.PushBack({ 32*i, 32*spellId, 32, 32 });
		}
		anim.loop = false;
		anim.hasIdle = false;
	}

	treeFeedback(CommonUpgrades type, int currentFrame, iPoint pos, int id)
	{
		this->currentFrame = currentFrame;
		this->pos = pos;
		this->id = id;

		for (int i = 0; i < 2; i++)
		{
			anim.PushBack({ 48 * i, 16 * ((int)type-1), 48, 16 });
		}
		anim.loop = false;
		anim.hasIdle = false;
	}
};

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

	void GamepadInputController(bool isSkillTree);

	void TextCleaning();

private:

	int startId = 0;

	bool active = false;
	int btnSize = 36;

	SDL_Texture* treeTexture = nullptr;
	SDL_Texture* feedbackTex = nullptr; 
	SDL_Texture* cmmFeedbackTex = nullptr;
	SDL_Texture* commonTreeTexture = nullptr;

	SDL_Rect bRect = { 0, 0, 0, 0 };

	iPoint bPoint = { 0, 0 };
	iPoint treeSwitchPoint = { 0, 0 };

	PlayerClass pClass;
	ClassTree* tree;
	CommonTree* cTree;
	Player* player;

	GUIButton* treeSwitch = nullptr;

    //GUIButton* testBtn = nullptr;
	List<GUIButton*>* unlockBtn = nullptr;
	List<treeFeedback*>* feed = nullptr;

	List<GUIButton*>* cmmUnlockBtn = nullptr;
	List<treeFeedback*>* cmmFeed = nullptr;

	bool switcher = true;

	// Gamepad implementation

	int currentSelectedOption = 0;

	Text* switchButtonText;
public:
	bool isFirstFrame = false;
};

#endif // !__CLASDD_TREE_HUD_H__