#include "SmallMario.h"
#include "Game.h"
#include "AnimationDatabase.h"

void SmallMario::Awake()
{
	SetTag(ObjectTags::SmallMario);
	DebugOut(L"Mario Awake\n");

	InitAnimations();

	Collider2D* collider = new Collider2D();
	collider->SetBoxSize(MARIO_SMALL_BBOX);
	collider->AttachToEntity(this);
	collider->SetName("Normal");
	this->colliders->push_back(collider);

	canCrouch = false;
}

void SmallMario::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	
	AddAnimation("Run",	 animations->Get("ani-small-mario-run"));
	AddAnimation("Walk", animations->Get("ani-small-mario-walk"));
	AddAnimation("Jump", animations->Get("ani-small-mario-jump"));
	AddAnimation("Fly",	 animations->Get("ani-small-mario-high-jump"));
	AddAnimation("Fall", animations->Get("ani-small-mario-jump"));
	AddAnimation("Idle", animations->Get("ani-small-mario-idle"));
	AddAnimation("Skid", animations->Get("ani-small-mario-skid"));
	// AddAnimation("Crouch", animations->Get("ani-small-mario-"));
}
