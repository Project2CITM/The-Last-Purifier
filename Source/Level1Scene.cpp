#include "Level1Scene.h"
#include "PlayerController.h"
#include "Particle.h"
#include "PlayerRevenant.h"
#include "PlayerSage.h"
#include "PlayerCombat.h"
#include "ModuleInput.h"
#include "SpellInfo.h"
#include "Text.h"
#include "ModuleAudio.h"
#include "CommonTree.h"
#include "SpellSpawnManager.h"
#include "ModuleWindow.h"
#include "Ghoul.h"
#include "Kaboom.h"
#include "Worm.h"
#include "Slime.h"
#include "PlayerStats.h"
#include "Souls.h"
#include "ButtonPuzzle.h"
#include "WeaponObject.h"
#include "Column.h"
#include "WeaponChoosingHUD.h"
#include "Boss.h"
#include "MenuManager.h"


Level1Scene::Level1Scene() : SceneGame("testScene")
{

}

bool Level1Scene::Start()
{
    PlayerClass playerClass;

    pugi::xml_document playerStats;

    pugi::xml_parse_result result = playerStats.load_file("PlayerStats.xml");

    if (result == NULL)
    {
        LOG("Could not load xml file: %s. pugi error: %s", "PlayerStats.xml", result.description());
    }
    else
    {
        playerClass = (PlayerClass)playerStats.child("stats").child("currentClass").attribute("class").as_int();
    }

    if (playerClass == PlayerClass::REVENANT) player = new PlayerRevenant();
    
    else player = new PlayerSage();

    spawnManager = SpellSpawnManager::GetInstance();

    hudInGame = new HUDInGame();

    hudInGame->Start();

    roomManager.Start();

    Scene::Start();

    menuManager = MenuManager::GetInstance();

    weaponChoosingHUD = new WeaponChoosingHUD(playerClass);
    weaponChoosingHUD->Start();

    app->renderer->camera->SetPosition(player->controller->GetPosition());

    app->renderer->camera->SetTarget(player->controller);

    for (int i = 0; i < roomManager.rooms.count(); i++)
    {
        if (roomManager.rooms[i]->id == -6)
        {
            new ButtonPuzzle(roomManager.rooms[i]);
        }
    }
    app->audio->PlayMusic("Audio/Ambience/amb_dungeon1_2.ogg", 2.0f, false);

    return true;
}

bool Level1Scene::PreUpdate()
{
    roomManager.PreUpdate(player->controller->GetPosition());

    hudInGame->PreUpdate();

    weaponChoosingHUD->PreUpdate();

    menuManager->PreUpdate();

    Scene::PreUpdate();

    return true;
}

void Level1Scene::OptickUpdate()
{

}

bool Level1Scene::Update()
{
    roomManager.Update(player->controller->GetPosition());

    hudInGame->Update();

    Scene::Update();

    return true;
}

bool Level1Scene::PostUpdate()
{
    roomManager.PostUpdate(player->controller->GetPosition());

    hudInGame->PostUpdate();

    weaponChoosingHUD->PostUpdate();

    Scene::PostUpdate();

    return true;
}

bool Level1Scene::CleanUp()
{
    app->events->TriggerEvent(GameEvent::DELETING_SCENE);

    if (player != nullptr)
    {
        player->CleanUp();
        RELEASE(player);
    }

    if (hudInGame != nullptr)
    {
        hudInGame->CleanUp();
        RELEASE(hudInGame);
    }
    roomManager.CleanUp();

    if (weaponChoosingHUD != nullptr)
    {
        weaponChoosingHUD->CleanUp();
        RELEASE(weaponChoosingHUD);
    }

    spawnManager->ReleaseInstance();
    menuManager->ReleaseInstance();
    Scene::CleanUp();
    return false;
}

void Level1Scene::AddGUIPause(GUI* gui)
{
    hudInGame->AddGUIPause(gui);
}

void Level1Scene::AddGUIControls(GUI* gui)
{
    hudInGame->AddGUIControls(gui);
}

void Level1Scene::AddGUISettingsP(GUI* gui)
{
    hudInGame->AddGUISettingsP(gui);
}