#include "ShrinkDownFX.h"
#include "Game.h"
#include "AnimationDatabase.h"

void ShrinkDownFX::Awake()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Clone("ani-mario-shrink-down"), false);
}
