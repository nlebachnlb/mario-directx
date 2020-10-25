#pragma once
#include "IState.h"

class StateMachine
{
public:
	void SwitchState(IState* destState);
	void Execute();
protected:
	IState* currentState;
};

