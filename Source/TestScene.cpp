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
#include "EnemyDummy.h"
#include "SpellSpawnManager.h"
#include "ModuleWindow.h"
#include "Ghoul.h"

TestScene::TestScene() : SceneGame("testScene")
{

}

TestScene::~TestScene()
{
}

bool TestScene::Start()
{
    //advisorString = "hi";

    player = new PlayerSage();

    spawnManager = SpellSpawnManager::GetInstance();

    //app->renderer->camera->SetTarget(player->controller);
    //player = new PlayerSage(app);
    //playerController = new PlayerController("test", "test", app);
    //gameObjects.add(playerController); // Ahora se anade automatico a la lista
    app->renderer->camera->SetTarget(player->controller);

    //Test Skill/Spell tree
    revenantTree = ClassTree::GetInstance();

    // Test particle
    Particle* p = new Particle({ 0,0 }, 2, 0, { 1,0 });
    p->renderObjects[0].InitAsRect({ p->GetPosition().x,p->GetPosition().y,50,50 }, { 0,255,0,255 }, true, 3);

    // Test text
    //t =  new Text({ 0,0 },"");
    //t->SetText("Hello world");
    //advisor = new Text({ 0,0 },"","defaultFont");
    //t->SetColor({ 255,255,0,100 });

    hudInGame = new HUDInGame();
    hudInGame->Start();
    roomManager.Start();

    classTreeHud = new ClassTreeHud();
    classTreeHud->Start();

    //new NPC("purifier1", { 0,0 });
    //NPC* npc1 = new NPC("purifier10", { 300,150 });
    //NPC* npc2 =new NPC("purifier10", { 40,100 });

    Scene::Start();

    spawnManager->SpawnSpell(player->controller->GetPosition() + iPoint(-40, 0));
    spawnManager->SpawnSpell(player->controller->GetPosition() + iPoint(-80, 0));
     
    new Ghoul(player->controller->GetPosition() + iPoint(-100, 0));

   

    return true;
}

bool TestScene::PreUpdate()
{
    if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
    {
        roomManager.CleanUp();
        roomManager.Start();
    }
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
    // Test Code--------------
    //if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
    //{
    //    Application::GetInstance()->testingNum = 20;
    //    printf("%d", Application::GetInstance()->testingNum);
    //}

    if (app->input->GetControllerButton(BUTTON_A) == KEY_DOWN)
    {
        printf("A");
    }

    if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || app->input->GetControllerButton(BUTTON_START) == KEY_DOWN) app->TogglePause(!app->isPause);


    //printf("Axis Left: X: %d Y: %d\n", app->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTX), app->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTY));

    hudInGame->PreUpdate();
    revenantTree->PreUpdate();
    classTreeHud->PreUpdate();

    //printf("%d  %d \n", player->controller->GetPosition().x, player->controller->GetPosition().y);
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
    revenantTree->Update();
    classTreeHud->Update();
    Scene::Update();
    return true;
}

bool TestScene::PostUpdate()
{
    //Test draw renderObject
    //RenderObject ro;
    //ro.InitAsRect(SDL_Rect{ 50,50,50,50 }, SDL_Color{ 0,0,255,255 }, true, 2, 50);
    //ro.InitAsLine(iPoint{ 50,50 }, iPoint{ 100,50 }, true, SDL_Color{ 255,255,255,255 }, 2, 50);
    //ro.InitAsCircle(iPoint{ 50,50 }, 50, SDL_Color{ 255,255,255,255 }, 2, 50);
    //app->renderer->AddRenderObjectRenderQueue(ro);

    // Test draw circle
    //app->renderer->AddCircleRenderQueue(iPoint{ 50,50 }, 50, SDL_Color{ 255,255,255,255 }, 2, 50);

    // Test draw line
    //app->renderer->AddLineRenderQueue(iPoint{ 50,50 }, iPoint{ 100,50 }, true, SDL_Color{ 255,255,255,255 }, 2, 50);

    //Test draw rect
    //app->renderer->AddRectRenderQueue(SDL_Rect{ 50,50,50,50 }, SDL_Color{ 0,0,255,255 }, true, 2, 50);
    roomManager.PostUpdate();
    revenantTree->PostUpdate();
    classTreeHud->PostUpdate();
    //app->physics->ShapesRender();
    hudInGame->PostUpdate();
    Scene::PostUpdate();
    return true;
}

bool TestScene::CleanUp()
{
    if (player != nullptr)
    {
        player->CleanUp();
        RELEASE(player);
    }

    //advisor->pendingToDelate = true;
    //t->pendingToDelate = true;
    //sentence[1].clear();
    if (hudInGame != nullptr)
    {
        hudInGame->CleanUp();
        RELEASE(hudInGame);
    }
    if (classTreeHud != nullptr)
    {
        classTreeHud->CleanUp();
        RELEASE(classTreeHud);
    }
    roomManager.CleanUp();

    spawnManager->ReleaseInstance();

    revenantTree->ReleaseInstance();

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
