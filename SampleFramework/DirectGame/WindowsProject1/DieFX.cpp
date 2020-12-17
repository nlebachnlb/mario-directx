#include "DieFX.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "PhysicConstants.h"
#include "Mathf.h"
#include "MainCanvas.h"

void DieFX::Awake()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-small-mario-die"));
	rigidbody->SetDynamic(false);
}

void DieFX::Start()
{
	SetState("Default");
	fxState = 0;
	timer = 0;
}

void DieFX::Update()
{
	auto dt = Game::DeltaTime();
	switch (fxState)
	{
	case 0:
	{
		timer += dt;
		if (timer > 800)
		{
			timer = 0;
			fxState = 1;
			rigidbody->SetVelocity(&Vector2(0, -BOUNCE_FORCE));
			rigidbody->SetGravity(DEFAULT_GRAVITY / 1.65f);
			pos = transform.Position;
		}
	}
	break;
	case 1:
	{
		transform.Position = transform.Position + rigidbody->GetVelocity() * dt;

		auto vel = rigidbody->GetVelocity();
		vel.y += rigidbody->GetGravity() * dt;
		vel.y = Mathf::Min(vel.y, DEFAULT_FALL_LIMIT_VEL);
		rigidbody->SetVelocity(&vel);
	}
	break;
	}
}
