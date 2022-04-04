#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "Scene.h"

class GUIButton;
class GUISlider;

enum class CurrentMenu
{
	Main,
	Options,
	Credtis
};

class MainMenu : public Scene
{
public:

	// Constructor
	MainMenu();

	// Destructor
	~MainMenu();

	virtual bool InitScene();

	bool Start() override;

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

	bool CleanUp() override;


private:
	GUIButton* PlayBUT = nullptr;
	GUIButton* OptionsBUT = nullptr;
	GUIButton* CreditBUT = nullptr;
	GUIButton* ExitBUT = nullptr;
	GUIButton* LinkBUT = nullptr;

	GUIButton* CloseOptBUT = nullptr;
	GUIButton* SliderBUT = nullptr;
	
	GUIButton* CloseCrdBUT = nullptr;

	GUISlider* MusicSlider = nullptr;

	CurrentMenu currentMenu;
};

#endif //__MAIN_MENU_H__
