#ifndef _EVENT_LISTENER_
#define _EVENT_LISTENER

#define MAX_EVENTS_PER_EVENTLISTENER 4

enum class GameEvent
{
	NONE,
	PLAYER_ATTACK,
	SAVE_GAME,
	COMPLETE_ROOM,
	PLAYER_HIT,
	PLAYER_DIE,
	DELETING_SCENE,
	UPDATE_COMMON_TREE,
	UPDATE_PLAYER_STATS,
	MAX
};

class Application;

/// <summary>
/// This class should ALWAYS be created as a pointer, and added to the ModuleEvent list using app->events->AddListener()!!!!
/// </summary>
class EventListener
{
public:

	virtual void GameEventTriggered(GameEvent id) = 0;

	virtual ~EventListener() {};

	GameEvent listenTo[MAX_EVENTS_PER_EVENTLISTENER];
};
#endif

