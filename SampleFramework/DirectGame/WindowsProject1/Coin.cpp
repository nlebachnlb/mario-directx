#include "Coin.h"
#include "Game.h"
#include "AnimationDatabase.h"

void Coin::Awake()
{
	AbstractItem::Awake();
	SetTag(ObjectTags::Coin);

	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-coin"));
}

void Coin::Start()
{
	sprouting = false;
	visualRelativePosition = VectorZero();
	rigidbody->SetDynamic(false);
	colliders->at(0)->SetTrigger(true);
	SetState("Default");
}

void Coin::LateUpdate()
{
}

void Coin::PreRender()
{
}
