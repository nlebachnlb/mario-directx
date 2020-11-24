#include "SmokeSpotFX.h"
#include "AnimationDatabase.h"
#include "Game.h"

void SmokeSpotFX::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Clone("ani-fireball-damaged"), false);
}
