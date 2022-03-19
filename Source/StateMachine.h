#ifndef _STATE_MACHINE_
#define _STATE_MACHINE_

#include "Globals.h"
#include "List.h"
#include <string>

struct StateObject
{
	std::string name; //For debug purposes
	int priority;
	int totalFrames;
	int currentFrames;
};

class StateMachine
{
public:
	StateMachine();

	/// <summary>
	/// Adds a new StateObject to the State Machine. 
	/// </summary>
	/// <param name="name">For debugging purposes only.</param>
	/// <param name="priority">State priority. Higher priority states cannot be interrupted by lower priority ones.</param>
	/// <param name="frames">Number of frames this state lasts. Set to -1 if it is undefined.</param>
	/// <returns></returns>
	uint AddState(std::string name, int priority, int frames = -1);


	/// <summary>
	/// Updates states that have a frames time.
	/// </summary>
	void Update();

	/// <summary>
	/// Changes current state. If the current state has a higher priority and hasn't ended yet, it ignores this attempt.
	/// </summary>
	/// <param name="state"></param>
	void ChangeState(uint state);

	/// <summary>
	/// Returns current state
	/// </summary>
	/// <returns></returns>
	uint GetCurrentState();

private:

	void ChangeCurrentState(uint state);

	List<StateObject> states; // List of every state added to the State Machine
	uint currentState; // List index of the current state
};

#endif

