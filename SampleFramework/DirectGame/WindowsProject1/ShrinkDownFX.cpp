#include "ShrinkDownFX.h"
#include "Game.h"
#include "AnimationDatabase.h"

void ShrinkDownFX::Awake()
{
	rigidbody->SetDynamic(false);
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-mario-shrink-down"), false);
}
