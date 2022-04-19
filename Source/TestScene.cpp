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
#include "ParticleAttackRevenant.h"

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

    app->renderer->camera->SetPosition(player->controller->GetPosition());

    app->renderer->camera->SetTarget(player->controller);

    spawnManager->SpawnSpell(player->controller->GetPosition() + iPoint(-40, 0));
    spawnManager->SpawnSpell(player->controller->GetPosition() + iPoint(-80, 0));
    spawnManager->SpawnSpell(player->controller->GetPosition() + iPoint(-60, 0));
    spawnManager->SpawnSpell(player->controller->GetPosition() + iPoint(-20, 0));

    app->audio->PlayMusic("Assets/Audio/Ambience/amb_dungeon1_2.ogg");

    //new Kaboom(player->controller->GetPosition() + iPoint(-100, 0));

    //new Ghoul(player->controller->GetPosition() + iPoint(-100, 0));

    //new Worm(player->controller->GetPosition() + iPoint(-100, 0));

    //new Text(player->controller->GetPosition() + iPoint(-100, 0), "H");

    return true;
}

bool TestScene::PreUpdate()
{
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

    Scene::PreUpdate();
    return true;
}

bool TestScene::Update()
{
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
    roomManager.PostUpdate();

    hudInGame->PostUpdate();

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