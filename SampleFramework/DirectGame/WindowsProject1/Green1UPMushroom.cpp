#include "Green1UPMushroom.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "Mathf.h"

void Green1UPMushroom::Awake()
{
	AbstractItem::Awake();

	SetTag(ObjectTags::GreenMushroom);
	renderOrder = -1;
	rigidbody->SetMaterial(GREEN_MUSHROOM_PHYSIC_MATERIAL);

	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-1-up-mushroom"));
}

void Green1UPMushroom::Start()
{
	AbstractItem::Start();
	SetState("Default");
}

void Green1UPMushroom::OnSproutCompleted()
{
	rigidbody->SetGravity(GREEN_MUSHROOM_GRAVITY);
	rigidbody->SetVelocity(&Vector2(GREEN_MUSHROOM_SPEED * (Random::Range(0, 2) >= 1 ? 1 : -1), 0));
}
