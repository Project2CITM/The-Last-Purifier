#include "GameOverScene.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "HUDInGame.h"
#include "ModuleAudio.h"
#include "GUIButton.h"

GameOverScene::GameOverScene() : Scene("GameOverScene")
{
    app = Application::GetInstance();
}

GameOverScene::~GameOverScene()
{
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
    gameOverBG->InitAsTexture(app->textures->Load("Sprites/UI/GameOver/GameOverScreen.png"), { 0,0 }, { 0,0,0,0 }, 0.5f, 1, 0.9f);
    
    text = new Text({ 260,220 }, "");
    text2 = new Text({ 310,220 }, "");
    text5 = new Text({ 200,220 }, "");
    
    app->audio->PlayMusic("Audio/Music/Win-Lose/loseThemee.ogg", 2.0f, false);

    // GUI 

    retryButton = new GUIButton({ 200,300 }, 60, 16, app->textures->Load("Sprites/UI/Continue.png", false));
 
    mainMenuButton = new GUIButton({ 400,300 }, 60, 16, app->textures->Load("Sprites/UI/QuitBUT.png", false));

    return true;
}

bool GameOverScene::PreUpdate()
{
    retryButton->Update();
    mainMenuButton->Update();

    if (app->input->usingGameController)
    {
        if (app->input->GetControllerButton(JoystickButtons::BUTTON_RIGHT) == KEY_DOWN) selectedOption++;
        if (app->input->GetControllerButton(JoystickButtons::BUTTON_LEFT) == KEY_DOWN) selectedOption--;
         
        selectedOption = selectedOption < 0 ? selectedOption = 1 : selectedOption > 1 ? selectedOption = 0 : selectedOption = selectedOption;
        
        switch (selectedOption)
        {
        case 0:
            retryButton->HoverButton();
            if (app->input->GetControllerButton(JoystickButtons::BUTTON_A) == KEY_DOWN)
            {
                retryButton->PressButton(true);
            }
            break;
        case 1:
            mainMenuButton->HoverButton();
            if (app->input->GetControllerButton(JoystickButtons::BUTTON_A) == KEY_DOWN)
            {
                mainMenuButton->PressButton(true);
            }
            break;
        }

    }

    return true;
}

bool GameOverScene::Update()
{
    if (retryButton->doAction)
    {
        retryButton->doAction = false;
        app->scene->ChangeCurrentSceneRequest(SCENES::HUB, 30);
    }

    if (mainMenuButton->doAction)
    {
        mainMenuButton->doAction = false;
        app->scene->ChangeCurrentSceneRequest(SCENES::MAIN_MENU, 30);
    }
    
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

    retryButton->PostUpdate();
    mainMenuButton->PostUpdate();
   
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



