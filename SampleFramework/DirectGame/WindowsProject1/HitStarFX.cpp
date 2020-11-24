#include "HitStarFX.h"
#include "AnimationDatabase.h"
#include "Game.h"

void HitStarFX::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Clone("ani-enemy-damaged"), false);
}
