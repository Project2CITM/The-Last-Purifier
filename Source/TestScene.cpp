#include "TestScene.h"
#include "PlayerController.h"
#include "Particle.h"
#include "PlayerRevenant.h"
#include "PlayerSage.h"
#include "PlayerCombat.h"
#include "ModuleInput.h"
#include "ClassTree.h"
#include "SpellInfo.h"
#include "Text.h";

TestScene::TestScene():Scene("testScene")
{
}

TestScene::~TestScene()
{
}

bool TestScene::Start()
{
    
    player = new PlayerRevenant();
    //app->renderer->camera->SetTarget(player->controller);
    //player = new PlayerSage(app);
    //playerController = new PlayerController("test", "test", app);
    //gameObjects.add(playerController); // Ahora se anade automatico a la lista
    app->renderer->camera->SetTarget(player->controller);

    //Test Skill/Spell tree
    revenantTree = new ClassTree(PlayerClass::REVENANT);

    // Test particle
    Particle* p = new Particle({ 0,0 }, 2, 0, { 1,0 });
    p->renderObjects[0].InitAsRect({ p->GetPosition().x,p->GetPosition().y,50,50 }, { 0,255,0,255 }, true, 3);

    // Test text

    t =  new Text({ 0,0 },"");
    //t->SetText("Hello world");
    advisor = new Text({ 0,0 },"","defaultFont");
  //  t->SetColor({ 255,255,0,100 });



    roomManager.Start();
    chargeDialog();
    Scene::Start();

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

    if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
    {
        dialogEnable = true;
    }
    if (!dialogEnable) {
        advisor->SetText(advisorString);
    }
    if (dialogEnable) {
        advisor->SetText("");

        if (separador) {
            if (app->input->GetKey(SDL_SCANCODE_RETURN)) {

                dialogCont++;


                separador = false;
            }
        }
        else {
            separadorCont++;
        }
        if (separadorCont == 60) {
            separador = true;
            separadorCont = 0;
        }
        t->SetText(sentence[dialogCont]);

    }

    //printf("Axis Left: X: %d Y: %d\n", app->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTX), app->input->GetControllerAxis(SDL_CONTROLLER_AXIS_LEFTY));

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

    advisor->pendingToDelate = true;
    t->pendingToDelate = true;
    //sentence[1].clear();

    roomManager.CleanUp();
    Scene::CleanUp();

    return false;
}
void TestScene::chargeDialog() {
    configDialog = app->config.child("dialogText");

    sentence[1] = configDialog.child("Sentence1").child_value();
    sentence[2] = configDialog.child("Sentence2").child_value();

    advisorString = configDialog.child("advisor").child_value();
}