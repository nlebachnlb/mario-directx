#include "AbstractPlatform.h"
#include "PhysicConstants.h"
#include "Game.h"
#include "Mathf.h"

void AbstractPlatform::Awake()
{
	SetTag(ObjectTags::Platform);
	renderOrder = 3;

	Collider2D* box = new Collider2D();
	box->SetBoxSize(GetBoxSize());
	box->AttachToEntity(this);
	this->colliders->push_back(box);
	this->rigidbody->SetDynamic(false);
	rigidbody->SetGravity(0);

	InitAnimations();
}

void AbstractPlatform::Start()
{
	SetState("Default");
}

void AbstractPlatform::Update()
{
	
}

void AbstractPlatform::OnOffScreen()
{
}

void AbstractPlatform::OnTouch()
{
}

void AbstractPlatform::ApplyGravity()
{
	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	transform.Position = transform.Position + rigidbody->GetVelocity() * dt;

	auto vel = rigidbody->GetVelocity();
	vel.y += rigidbody->GetGravity() * dt;
	vel.y = Mathf::Min(vel.y, DEFAULT_FALL_LIMIT_VEL);
	rigidbody->SetVelocity(&vel);
}
