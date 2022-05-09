#pragma once
#include "Scene.h"
#include "ModuleEvents.h"

class GameOverScene :
    public Scene,public EventListener
{
public:

    GameOverScene();

    ~GameOverScene();

    bool InitScene() override;

    bool Start() override;

    bool PreUpdate() override;

    bool Update() override;

    bool PostUpdate() override;

    bool CleanUp() override;

    void GameEventTriggered(GameEvent id) override;

private:

    Text* text = nullptr;

    SDL_Texture* gameOverBG = nullptr;

    SDL_Texture* gameOverTitle = nullptr;

    Application* app;

};

