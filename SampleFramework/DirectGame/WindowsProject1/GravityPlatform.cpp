#include "GravityPlatform.h"
#include "Game.h"
#include "AnimationDatabase.h"

void GravityPlatform::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();

	AddAnimation("Default", animations->Get("ani-platform"));
}

Vector2 GravityPlatform::GetBoxSize()
{
	return Vector2(48 * 3, 44);
}

void GravityPlatform::Update()
{
	ApplyGravity();
}
