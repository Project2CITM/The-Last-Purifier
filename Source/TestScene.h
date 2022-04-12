#include "Scene.h"

#include "RoomManager.h"
#include "HUDInGame.h"
#include "NPC.h"
#include "ClassTreeHud.h"

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
	HUDInGame* hudInGame = nullptr;
	ClassTreeHud classTreeHud;
	//PlayerRevenant* player;
	Player* player = nullptr;

	ClassTree* revenantTree = nullptr; //Test skill/spell tree

	//Dialog
	/*pugi::xml_node configDialog;
	bool dialogEnable = false;
	bool separador = false;
	int separadorCont = 0;
	int dialogCont = 1;*/
};
