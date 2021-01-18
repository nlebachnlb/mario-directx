#include "Coin.h"
#include "Game.h"
#include "AnimationDatabase.h"

void Coin::Awake()
{
	isStatic = true;
	AbstractItem::Awake();
	SetTag(ObjectTags::Coin);

	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-coin"));
	AddAnimation("Freeze", animations->Get("ani-idle-coin"));
}

void Coin::Start()
{
	sprouting = false;
	timeFreeze = false;
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
	if (timeFreeze) { if (currentState.compare("Freeze") != 0) SetState("Freeze"); }
	else if (currentState.compare("Default") != 0) SetState("Default");
}

void Coin::SetFreeze(bool value)
{
	timeFreeze = value;
}
