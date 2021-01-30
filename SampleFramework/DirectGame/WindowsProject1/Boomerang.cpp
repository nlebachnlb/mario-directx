#include "Boomerang.h"
#include "Game.h"
#include "AnimationDatabase.h"

void Boomerang::Awake()
{
	global = true;
	SetTag(ObjectTags::HostileProjectiles);

	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-boomerang-spin"));

	Collider2D* box = new Collider2D();
	box->AttachToEntity(this);
	box->SetBoxSize(BOOMERANG_BBOX);
	box->SetTrigger(true);
	this->colliders->push_back(box);

	this->rigidbody->SetDynamic(false);
	this->rigidbody->SetGravity(0);
	renderOrder = 15;
}

void Boomerang::Start()
{
	SetState("Default");
	phase = 0;
	auto vel = Vector2(BOOMERANG_THROWN_VEL.x * facing, BOOMERANG_THROWN_VEL.y);
	rigidbody->SetVelocity(&vel);
}

void Boomerang::Update()
{
	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	transform->Position = transform->Position + rigidbody->GetVelocity() * dt;
	auto v = rigidbody->GetVelocity();

	if (transform->Position.y <= owner->GetTransform().Position.y)
	{
		v.x += BOOMERANG_ACCE.x * facing;
		v.y += BOOMERANG_ACCE.y;
	}
	else
	{
		if (phase == 0) phase = 1;
		if (facing > 0 && v.x < -BOOMERANG_TURN_BACK_VEL.x)
			v.x = -BOOMERANG_TURN_BACK_VEL.x;
		else if (facing < 0 && v.x > BOOMERANG_TURN_BACK_VEL.x)
			v.x = BOOMERANG_TURN_BACK_VEL.x;
		v.y = 0;
	}
	
	rigidbody->SetVelocity(&v);
}

void Boomerang::OnOffScreen()
{
	pool->Revoke(this);
}

void Boomerang::SetPool(ObjectPool* pool)
{
	this->pool = pool;
}

void Boomerang::OnOverlapped(Collider2D* self, Collider2D* other)
{
	if (other->GetGameObject()->GetTag() == owner->GetTag() && phase == 1)
	{
		phase = 0;
		pool->Revoke(this);
	}
}
