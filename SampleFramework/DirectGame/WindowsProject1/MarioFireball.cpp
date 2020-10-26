#include "MarioFireball.h"
#include "Game.h"
#include "AnimationDatabase.h"

void MarioFireball::Awake()
{
	DebugOut(L"Fireball Awake\n");
	SetTag(ObjectTags::FriendlyProjectiles);

	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-fire-ball"));

	Collider2D* box = new Collider2D();
	box->AttachToEntity(this);
	box->SetBoxSize(FIREBALL_BBOX);
	box->SetTrigger(true);
	this->colliders->push_back(box);
	this->rigidbody->SetDynamic(true);
}

void MarioFireball::Start()
{
	SetState("Default");
}
