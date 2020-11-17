#include "GrowUpFX.h"
#include "AnimationDatabase.h"
#include "Game.h"

void GrowUpFX::Awake()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Clone("ani-mario-grow-up"), false);
}
