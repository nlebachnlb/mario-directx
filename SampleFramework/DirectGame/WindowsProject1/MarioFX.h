#pragma once
#include "FXObject.h"
#include "PlayerController.h"
#include "IState.h"

class MarioFX : public FXObject, public IState
{
public:
	void SetController(PlayerController* controller);
	void Entrance() override;
	void Execute() override;
	void Exit() override;

protected:
	PlayerController* controller;
};

