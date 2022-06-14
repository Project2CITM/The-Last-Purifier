#include "WinScreen.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "GUIButton.h"

WinScene::WinScene() : Scene("WinScene")
{
    app = Application::GetInstance();
}

bool WinScene::Start()
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

    winBG = new RenderObject();
    pressKeyToReturnToHub[0] = new RenderObject();
    pressKeyToReturnToHub[1] = new RenderObject();
    soul = new RenderObject();
    soul->InitAsTexture(app->textures->Load("Sprites/Soul/soul.png"), { 390, 207 }, { 0,0,50,89 }, 0.25f, 4, 0, 0, SDL_FLIP_NONE, 0);
    winBG->InitAsTexture(app->textures->Load("Assets/Sprites/UI/GameOver/WinScreen.png"), { 0,0 }, { 0,0,0,0 }, 0.5f, 1, 0.9f);
  /*  pressKeyToReturnToHub[0]->InitAsTexture(app->textures->Load("Assets/Sprites/UI/GameOver/PressKeyToTryAgain.png"), { 190,270 }, { 0,0,538,64 }, 0.5f);
    pressKeyToReturnToHub[1]->InitAsTexture(app->textures->Load("Assets/Sprites/UI/GameOver/PressKeyToTryAgain.png"), { 190,270 }, { 0,72,538,64 }, 0.5f);*/

    text = new Text({ 355,220 }, "");
    text2 = new Text({ 405,220 }, "");
    text5 = new Text({ 180,220 }, "");

    app->audio->PlayMusic("Audio/Music/Win-Lose/winThemee.ogg", 2.0f, false);

    // GUI 

    retryButton = new GUIButton({ 200,300 }, 60, 16, app->textures->Load("Assets/Sprites/UI/Continue.png", false));

    mainMenuButton = new GUIButton({ 400,300 }, 60, 16, app->textures->Load("Assets/Sprites/UI/QuitBUT.png", false));

	return true;
}

bool WinScene::PreUpdate()
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

bool WinScene::Update()
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

    text5->SetText("Congratulations You have ");
    text2->SetText("souls ");
    text->SetText(std::to_string(score));
    Scene::Update();
	return true;
}

bool WinScene::PostUpdate()
{
    retryButton->PostUpdate();
    mainMenuButton->PostUpdate();

    app->renderer->AddRenderObjectRenderQueue(*winBG);
    app->renderer->AddRenderObjectRenderQueue(*soul);
    if (app->input->usingGameController)
    {
        app->renderer->AddRenderObjectRenderQueue(*pressKeyToReturnToHub[0]);
    }
    else
    {
        app->renderer->AddRenderObjectRenderQueue(*pressKeyToReturnToHub[1]);
    }

    Scene::PostUpdate();

	return true;
}

bool WinScene::CleanUp()
{
    RELEASE(winBG);
    RELEASE(pressKeyToReturnToHub[0]);
    RELEASE(pressKeyToReturnToHub[1]);
    RELEASE(soul);
    Scene::CleanUp();
	return true;
}
