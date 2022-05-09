#include "SceneGame.h"

#include "RoomManager.h"
#include "HUDInGame.h"
#include "NPC.h"
#include "ClassTreeHud.h"

class SpellSpawnManager;

class TestScene :  public SceneGame
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

	void OptickUpdate();

private:

	RoomManager roomManager;

	HUDInGame* hudInGame = nullptr;

	SpellSpawnManager* spawnManager = nullptr;
};