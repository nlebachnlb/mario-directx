#include "RedMushroom.h"
#include "Mathf.h"
#include "Game.h"
#include "AnimationDatabase.h"

void RedMushroom::Awake()
{
	AbstractItem::Awake();

	SetTag(ObjectTags::RedMushroom);
	renderOrder = -1;
	rigidbody->SetMaterial(MUSHROON_PHYSIC_MATERIAL);

	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-super-mushroom"));
}

void RedMushroom::Start()
{
	AbstractItem::Start();
	SetState("Default");
}

void RedMushroom::OnSproutCompleted()
{
	rigidbody->SetGravity(MUSHROOM_GRAVITY);
	rigidbody->SetVelocity(&Vector2(MUSHROOM_SPEED * (Random::Range(0, 2) >= 1 ? 1 : -1), 0));
}
