#include "MarioFireball.h"
#include "Game.h"
#include "AnimationDatabase.h"

void MarioFireball::Awake()
{
	SetTag(ObjectTags::FriendlyProjectiles);

	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-fire-ball"));
}

void MarioFireball::Start()
{
	SetState("Default");
}
