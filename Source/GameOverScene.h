#pragma once
#include "Scene.h"
#include "ModuleEvents.h"

class GameOverScene :
    public Scene,public EventListener
{

    bool InitScene() override;

    bool Start() override;

    bool Update() override;

    bool PostUpdate() override;

    bool CleanUp() override;

    void GameEventTriggered(GameEvent id) override;

private:

    Text* text = nullptr;

    SDL_Texture* gameOverBG = nullptr;

    SDL_Texture* gameOverTitle = nullptr;

};

