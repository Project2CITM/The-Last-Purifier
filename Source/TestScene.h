#include "Scene.h"

#include "RoomManager.h"
#include "HUDInGame.h"
#include "NPC.h"

class PlayerRevenant;
class PlayerSage;
class ClassTree;

class TestScene :  public Scene
{
public:

	TestScene();

	~TestScene();

	bool Start() override;

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

	bool CleanUp() override;


	void AddGUIPause(GUI* gui) override;

	void AddGUIControls(GUI* gui) override;
	
	void AddGUISettingsP(GUI* gui) override;

private:

	RoomManager roomManager;
	HUDInGame hudInGame;
	
	//PlayerRevenant* player;
	PlayerSage* player;

	ClassTree* revenantTree; //Test skill/spell tree7

	//Dialog
	pugi::xml_node configDialog;
	bool dialogEnable = false;
	bool separador = false;
	int separadorCont = 0;
	int dialogCont = 1;

	//string advisorString;
	//std::string advisorString = "";


	//std::string sentence3 = "";
};

