#include "TestScene.h"
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

#include "External/Optick/include/optick.h"

TestScene::TestScene() : SceneGame("testScene")
{

}

TestScene::~TestScene()
{
}

bool TestScene::Start()
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
 
    weaponChoosingHUD = new WeaponChoosingHUD(playerClass);
    weaponChoosingHUD->Start();

    app->renderer->camera->SetPosition(player->controller->GetPosition());

    app->renderer->camera->SetTarget(player->controller);

    //new Slime((player->controller->GetPosition() + iPoint{ 40, 40 }));

    //spawnManager->SpawnSpell(player->controller->GetPosition() + iPoint(-40, 0));
    //spawnManager->SpawnSpell(player->controller->GetPosition() + iPoint(-80, 0));
    //spawnManager->SpawnSpell(player->controller->GetPosition() + iPoint(-60, 0));
    //spawnManager->SpawnSpell(player->controller->GetPosition() + iPoint(-20, 0));

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

bool TestScene::PreUpdate()
{
    OPTICK_EVENT();
   /* if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
    {
        roomManager.CleanUp();
        roomManager.Start();
    }*/
    // Test Code-------------
    if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
    {
        SpellInfo spell;
        spell.id = (SpellID)((rand() % 10) + (1));
        spell.spellLevel = rand() % 4;
        if (!player->controller->combat->AddSpell(spell))
        {
            printf("No more empty spell nor deck Slots!!\n");
        }
    }

    if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->GetControllerButton(BUTTON_START) == KEY_DOWN) app->TogglePause(!app->isPause);
    
    roomManager.PreUpdate(player->controller->GetPosition());

    hudInGame->PreUpdate();

    weaponChoosingHUD->PreUpdate();

    Scene::PreUpdate();

    return true;
}

void TestScene::OptickUpdate()
{
    OPTICK_EVENT();
}

bool TestScene::Update()
{
    OPTICK_EVENT();
    //if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
    //    roomManager.mapMovement.x -= 10;

    //if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
    //    roomManager.mapMovement.x += 10;

    //if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
    //    roomManager.mapMovement.y -= 10;

    //if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
    //    roomManager.mapMovement.y += 10;

   
    roomManager.Update(player->controller->GetPosition());

    hudInGame->Update();

    Scene::Update();

    return true;
}

bool TestScene::PostUpdate()
{
    roomManager.PostUpdate(player->controller->GetPosition());

    hudInGame->PostUpdate();

    weaponChoosingHUD->PostUpdate();

    Scene::PostUpdate();

    return true;
}

bool TestScene::CleanUp()
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

    weaponChoosingHUD->CleanUp();
    RELEASE(weaponChoosingHUD);

    spawnManager->ReleaseInstance();

    Scene::CleanUp();
    return false;
}

void TestScene::AddGUIPause(GUI* gui)
{
    hudInGame->AddGUIPause(gui);
}

void TestScene::AddGUIControls(GUI* gui)
{
    hudInGame->AddGUIControls(gui);
}

void TestScene::AddGUISettingsP(GUI* gui)
{
    hudInGame->AddGUISettingsP(gui);
}