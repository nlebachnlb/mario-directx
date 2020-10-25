#pragma once
class IState
{
public:
	virtual void Entrance() = 0;
	virtual void Execute() = 0;
	virtual void Exit() = 0;
};

