#include "MenuVersionFX.h"
#include "Game.h"
#include "AnimationDatabase.h"

void MenuVersionFX::Awake()
{
	rigidbody->SetDynamic(false);
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-version"));

	SetState("Default");
}
