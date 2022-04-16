#ifndef _EVENT_LISTENER_
#define _EVENT_LISTENER

enum class GameEvent
{
	PLAYER_ATTACK,
	SAVE_GAME,
};

class Application;

/// <summary>
/// This class should ALWAYS be created as a pointer, and added to the ModuleEvent list using app->events->AddListener()!!!!
/// </summary>
class EventListener
{
public:
	EventListener(GameEvent id);

	virtual void GameEventTriggered();

	~EventListener();

	GameEvent listenTo;

	Application* app = nullptr;
};
#endif

