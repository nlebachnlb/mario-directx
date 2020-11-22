#include "Coin.h"
#include "Game.h"
#include "AnimationDatabase.h"

void Coin::Awake()
{
	AbstractItem::Awake();
	SetTag(ObjectTags::Coin);

	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-idle-icon"));
}

void Coin::Start()
{
	sprouting = false;
	visualRelativePosition = VectorZero();
	rigidbody->SetDynamic(false);
	colliders->at(0)->SetTrigger(true);
	SetState("Default");
}
