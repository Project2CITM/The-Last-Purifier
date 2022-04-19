#ifndef _EVENT_LISTENER_
#define _EVENT_LISTENER

enum class GameEvent
{
	PLAYER_ATTACK,
	SAVE_GAME,
	COMPLETE_ROOM,
	PLAYER_HIT,
	DELETING_SCENE,
	MAX
};

class Application;

/// <summary>
/// This class should ALWAYS be created as a pointer, and added to the ModuleEvent list using app->events->AddListener()!!!!
/// </summary>
class EventListener
{
public:

	virtual void GameEventTriggered() = 0;

	virtual ~EventListener() {};

	GameEvent listenTo;
};
#endif

