#include "GravityPlatform.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "PhysicConstants.h"

void GravityPlatform::Start()
{
	AbstractPlatform::Start();
	touched = false;
}

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
	// ApplyGravity();
	if (!touched) rigidbody->SetVelocity(&startVelocity);
}

void GravityPlatform::OnTouch()
{
	if (touched) return;
	startVelocity.x = 0;
	rigidbody->SetVelocity(&VectorZero());
	rigidbody->SetGravity(DEFAULT_GRAVITY / 4.0f);
	touched = true;
}
