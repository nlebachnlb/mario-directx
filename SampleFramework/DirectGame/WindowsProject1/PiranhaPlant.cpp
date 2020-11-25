#include "PiranhaPlant.h"
#include "Game.h"
#include "AnimationDatabase.h"

void PiranhaPlant::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Reveal", animations->Get("ani-green-piranha-plant-attack"));
	AddAnimation("Idle", animations->Get("ani-green-piranha-plant-attack"));
}

void PiranhaPlant::Start()
{
	Plant::Start();

	waitTime = PIRANHA_WAIT_TIME;
	speed = PIRANHA_SPEED;
	hideDistance = PIRANHA_HIDE_DISTANCE;
}

Vector2 PiranhaPlant::GetBoxSize()
{
    return PIRANHA_BBOX;
}
