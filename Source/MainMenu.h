#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "Scene.h"

class GUIButton;
class GUISlider;
class GUICheckbox;

enum class CurrentMenu
{
	Main,
	Options,
	Credtis
};

enum class CurrentCredtis
{
	Creadors,
	Art,
	Audio,
	Otros
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
	GUIButton* MusicBUT = nullptr;
	GUIButton* fxBUT = nullptr;

	GUIButton* CloseCrdBUT = nullptr;

	GUIButton* CredtisCre = nullptr;
	GUIButton* CredtisAud = nullptr;
	GUIButton* CredtisArt = nullptr;
	GUIButton* CredtisOtr = nullptr;

	GUISlider* MusicSlider = nullptr;
	GUISlider* fxSlider = nullptr;

	GUICheckbox* FullScreenCHK;

	CurrentMenu currentMenu;
	CurrentCredtis currentCredtis;

	int ControllerPos = 0;
	int ControllerPosOpY = 0;
	int ControllerPosOpX = 0;
	int ControllerPosCr = 0;
	bool AxisPress = false;

	uint Appear_FX = NULL;

	uint Hover = NULL;
	uint Press = NULL;

	//load the texture
	void LoadTexFile(const pugi::xml_document& dataFile);

	//load the fx
	void LoadFxFile(const pugi::xml_document& dataFile);

	//load and play the music
	void LoadMusFile(const pugi::xml_document& dataFile);

	SDL_Texture* texture1 = nullptr;
};

#endif //__MAIN_MENU_H__
