#include "TransformationFX.h"
#include "Game.h"
#include "AnimationDatabase.h"

void TransformationFX::Awake()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();

	AddAnimation("Default", animations->Get("ani-mario-damaged"), false);
}

void TransformationFX::Start()
{
	SetState("Default");
}

void TransformationFX::OnAnimationEnd()
{
	DebugOut(L"Trans complete\n");
	if (controller != nullptr) controller->ContinueSwitchingState();
	pool->Revoke(this);
}
