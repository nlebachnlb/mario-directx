#include "PSwitch.h"
#include "Game.h"
#include "AnimationDatabase.h"

void PSwitch::Awake()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();

	AddAnimation("Default", animations->Get("ani-switch-block"));
	AddAnimation("Pressed", animations->Get("ani-switch-block-pressed"));

	renderOrder = -1;
}

void PSwitch::Start()
{
	SetState("Default");
}
