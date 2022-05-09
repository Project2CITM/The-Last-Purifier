#include "GameOverScene.h"
#include "ModuleEvents.h"
#include "ModuleInput.h"
#include "ModuleScene.h"

GameOverScene::GameOverScene() : Scene("GameOverScene")
{
    app = Application::GetInstance();
}

GameOverScene::~GameOverScene()
{
}

bool GameOverScene::InitScene()
{
    this->listenTo[0] = GameEvent::PLAYER_DIE;
    app->events->AddListener(this);

    return true;
}

bool GameOverScene::Start()
{
    return true;
}

bool GameOverScene::PreUpdate()
{

    if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || app->input->GetControllerButton(JoystickButtons::BUTTON_A) == KEY_DOWN)
    {
        app->scene->ChangeCurrentSceneRequest(SCENES::HUB, 60);
    }

    return true;
}

bool GameOverScene::Update()
{
    return true;
}

bool GameOverScene::PostUpdate()
{
    return true;
}

bool GameOverScene::CleanUp()
{

   app->events->RemoveListener(this);

    return true;
}

void GameOverScene::GameEventTriggered(GameEvent id)
{
    
}

