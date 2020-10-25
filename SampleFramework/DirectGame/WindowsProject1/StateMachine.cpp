#include "StateMachine.h"

void StateMachine::SwitchState(IState* destState)
{
	if (currentState != nullptr)
		currentState->Exit();

	currentState = destState;
	currentState->Entrance();
}

void StateMachine::Execute()
{
	if (currentState != nullptr)
		currentState->Execute();
}
