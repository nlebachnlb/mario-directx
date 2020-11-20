#include "GrowUpFX.h"
#include "AnimationDatabase.h"
#include "Game.h"

void GrowUpFX::Awake()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-mario-grow-up"), false);
}
