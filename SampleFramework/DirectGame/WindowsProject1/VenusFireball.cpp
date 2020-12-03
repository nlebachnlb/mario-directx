#include "VenusFireball.h"
#include "Game.h"
#include "AnimationDatabase.h"

void VenusFireball::Awake()
{
	SetTag(ObjectTags::HostileProjectiles);

	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-fire-ball"));

	Collider2D* box = new Collider2D();
	box->AttachToEntity(this);
	box->SetBoxSize(VENUS_FIREBALL_BBOX);
	box->SetTrigger(true);
	this->colliders->push_back(box);

	this->rigidbody->SetDynamic(false);
	this->rigidbody->SetGravity(0);
	renderOrder = 15;
}

void VenusFireball::Start()
{
	SetState("Default");
	// DebugOut(L"VFB: %f, %f\n", transform.Scale.x, transform.Scale.y);
}

void VenusFireball::Update()
{
	// DebugOut(L"Venus-fire-pos: %f, %f\n", transform.Position.x, transform.Position.y);
	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	transform.Position = transform.Position + rigidbody->GetVelocity() * dt;
}

void VenusFireball::OnOffScreen()
{
	DebugOut(L"Offscreen venus fireball\n");
	pool->Revoke(this);
}

void VenusFireball::SetPool(ObjectPool* pool)
{
	this->pool = pool;
}
