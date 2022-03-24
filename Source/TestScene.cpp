#include "TestScene.h"
#include "PlayerController.h"
#include "Particle.h"
#include "PlayerRevenant.h"
#include "PlayerSage.h"
#include "PlayerCombat.h"
#include "ModuleInput.h"

TestScene::TestScene():Scene("testScene")
{
}

TestScene::~TestScene()
{
}

bool TestScene::Start()
{
    player = new PlayerRevenant();
    //player = new PlayerSage(app);
    //playerController = new PlayerController("test", "test", app);
    //gameObjects.add(playerController); // Ahora se anade automatico a la lista

    // Test particle
    Particle* p = new Particle({ 0,0 }, 2, 0, { 1,0 });
    p->renderObjects[0].InitAsRect({ p->GetPosition().x,p->GetPosition().y,50,50 }, { 0,255,0,255 }, true, 3);

    roomManager.Start();

    Scene::Start();

    return true;
}

bool TestScene::PreUpdate()
{
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
    {
        roomManager.CleanUp();
        roomManager.Start();
    }   
    // Test Code-------------
    if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
    {
        if (!player->controller->combat->AddSpell((SpellID)((rand() % 10) + (1))))
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

    roomManager.Update();
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

    // Test draw rect  
    //app->renderer->AddRectRenderQueue(SDL_Rect{ 50,50,50,50 }, SDL_Color{ 0,0,255,255 }, true, 2, 50);

    roomManager.PostUpdate();
    //app->physics->ShapesRender();
   
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
    roomManager.CleanUp();
    Scene::CleanUp();

    return false;
}