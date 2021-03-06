#include "StateMachine.h"
#include "Application.h"

StateMachine::StateMachine()
{
	currentState = 0;
}

uint StateMachine::AddState(std::string name, int priority, int frames)
{
	StateObject state;
	state.name = name;
	state.priority = priority;
	state.totalTime = frames;
	state.currentTime = frames;

	states.add(state);

	return states.count()-1; // Return the list index of the added state.
}

void StateMachine::Update()
{
	stateTimer.Update();
	if (states[currentState].currentTime > 0)
	{
		states[currentState].currentTime -= stateTimer.getDeltaTime() * 1000;
	}
	stateTimer.Reset();
}

bool StateMachine::ChangeState(uint state)
{
	if (states[currentState].currentTime <= 0) // If current state had a time and it is out, we ignore priority.
	{
		ChangeCurrentState(state);
		return true;
	}
	if (states[currentState].priority <= states[state].priority) // If current state priority is the same or lower, we change state.
	{
		ChangeCurrentState(state);
		return true;
	}

	return false;
}

uint StateMachine::GetCurrentState()
{
	return currentState;
}

void StateMachine::ChangeCurrentState(uint state)
{
	currentState = state;

	if (states[currentState].totalTime > 0) // If the changed state had a frame time, we reset it.
	{
		states[currentState].currentTime = states[currentState].totalTime;
	}
}
