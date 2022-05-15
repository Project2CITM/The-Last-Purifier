#include "GameOverScene.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "HUDInGame.h"

GameOverScene::GameOverScene() : Scene("GameOverScene")
{
    app = Application::GetInstance();
}

GameOverScene::~GameOverScene()
{
}

bool GameOverScene::InitScene()
{
    Scene::InitScene();
    
    return true;
}

bool GameOverScene::Start()
{
    pugi::xml_document playerStats;

    pugi::xml_parse_result result = playerStats.load_file("PlayerStats.xml");
    if (result == NULL)
    {
        LOG("Could not load xml file: %s. pugi error: %s", "PlayerStats.xml", result.description());
    }
    else
    {
        this->score = playerStats.child("stats").child("common").child("souls").attribute("quantity").as_int();
    }
    
    gameOverBG = new RenderObject();
    pressKeyToTryAgain[0] = new RenderObject();
    pressKeyToTryAgain[1] = new RenderObject();
    souls = new RenderObject();
    souls->InitAsTexture(app->textures->Load("Sprites/Soul/soul.png"), { 295, 210 }, { 0,0,50,89 }, 0.25f, 4, 0, 0, SDL_FLIP_NONE, 0);
    gameOverBG->InitAsTexture(app->textures->Load("Assets/Sprites/UI/GameOver/GameOverScreen.png"), { 0,0 }, { 0,0,0,0 }, 0.5f, 1, 0.9f);
    pressKeyToTryAgain[0]->InitAsTexture(app->textures->Load("Assets/Sprites/UI/GameOver/PressKeyToTryAgain.png"), { 190,270 }, { 0,0,538,64 }, 0.5f);
    pressKeyToTryAgain[1]->InitAsTexture(app->textures->Load("Assets/Sprites/UI/GameOver/PressKeyToTryAgain.png"), { 190,270 }, { 0,72,538,64 }, 0.5f);
    
    text = new Text({ 260,220 }, "");
    text2 = new Text({ 310,220 }, "");
    text5 = new Text({ 200,220 }, "");
    

    return true;
}

bool GameOverScene::PreUpdate()
{

    if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || app->input->GetControllerButton(JoystickButtons::BUTTON_A) == KEY_DOWN)
    {
        app->scene->ChangeCurrentSceneRequest(SCENES::HUB, 30);
    }
    
    return true;
}

bool GameOverScene::Update()
{
    text5->SetText("You have ");
    text2->SetText("souls: ");
    text->SetText(std::to_string(score));
    Scene::Update();
    return true;
}

bool GameOverScene::PostUpdate()
{
   
    app->renderer->AddRenderObjectRenderQueue(*gameOverBG);
    app->renderer->AddRenderObjectRenderQueue(*souls);
    if (app->input->usingGameController == true)
    {
        app->renderer->AddRenderObjectRenderQueue(*pressKeyToTryAgain[0]);
    }
    else
    {
        app->renderer->AddRenderObjectRenderQueue(*pressKeyToTryAgain[1]);
    }
   
    Scene::PostUpdate();
    return true;
}

bool GameOverScene::CleanUp()
{
    RELEASE(gameOverBG);
    RELEASE(pressKeyToTryAgain[0]);
    RELEASE(pressKeyToTryAgain[1]);
    RELEASE(souls);
    Scene::CleanUp();
    return true;
}



