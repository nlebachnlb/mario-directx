#include "StateMachine.h"

void StateMachine::SwitchState(IState* destState)
{
	if (currentState != nullptr)
		currentState->Exit();

	currentState = destState;

	if (currentState != nullptr)
		currentState->Entrance();
}

void StateMachine::Execute()
{
	if (currentState != nullptr)
		currentState->Execute();
}
