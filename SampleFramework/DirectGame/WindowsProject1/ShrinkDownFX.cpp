#include "ShrinkDownFX.h"
#include "Game.h"
#include "AnimationDatabase.h"

void ShrinkDownFX::Awake()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-mario-shrink-down"), false);
}
