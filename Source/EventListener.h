#ifndef _EVENT_LISTENER_
#define _EVENT_LISTENER

enum class GameEvent
{
	PLAYER_ATTACK
};

class EventListener
{
public:
	EventListener(GameEvent id);

	virtual void GameEventTriggered();

	GameEvent listenTo;

};
#endif

