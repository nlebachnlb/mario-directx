#include "FireMario.h"
#include "MarioFireball.h"
#include "AnimationDatabase.h"
#include "Game.h"

void FireMario::Awake()
{
	CMario::Awake();

	SetTag(ObjectTags::PowerupMario);

	MarioFireball* fireball = Instantiate<MarioFireball>();
	fireball->LinkToPool(&fireballs);
	fireballs.Add(fireball);

	fireball = Instantiate<MarioFireball>();
	fireball->LinkToPool(&fireballs);
	fireballs.Add(fireball);
}

void FireMario::Start()
{
	CMario::Start();

	attacking = false;
}

void FireMario::OnKeyDown(int keyCode)
{
	CMario::OnKeyDown(keyCode);

	// Process attack key
	if (keyCode == marioKeySet.Attack && attacking == false && !fireballs.IsEmpty())
	{
		auto fireball = fireballs.Instantiate();
		if (fireball != nullptr)
		{
			attacking = true;
			fireball->SetPosition(transform.Position + Vector2(MARIO_BBOX.x / 2 * facing, 0));
			fireball->GetRigidbody()->SetVelocity(&Vector2(FIREBALL_VELOCITY * facing, FIREBALL_GRAVITY * Game::DeltaTime()));
			// DebugOut(L"Fireball created: %f, %f, %d\n", fireball->GetTransform().Position.x, fireball->GetTransform().Position.y, fireball->IsEnabled() ? 1 : 0);
		}
	}
}

void FireMario::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();

	AddAnimation("Run",		animations->Get("ani-fire-mario-high-speed"));
	AddAnimation("Walk",	animations->Get("ani-fire-mario-walk"));
	AddAnimation("Jump",	animations->Get("ani-fire-mario-jump"));
	AddAnimation("Fly",		animations->Get("ani-fire-mario-high-jump"));
	AddAnimation("Fall",	animations->Get("ani-fire-mario-fall"));
	AddAnimation("Idle",	animations->Get("ani-fire-mario-idle"));
	AddAnimation("Skid",	animations->Get("ani-fire-mario-skid"));
	AddAnimation("Crouch",	animations->Get("ani-fire-mario-crouch")); 
	AddAnimation("Attack",	animations->Get("ani-fire-mario-throw"), false);

	AddAnimation("HoldIdle", animations->Get("ani-fire-mario-hold-idle"));
	AddAnimation("HoldMove", animations->Get("ani-fire-mario-hold"));
	AddAnimation("HoldJump", animations->Get("ani-fire-mario-hold-jump"));
	AddAnimation("Kick", animations->Get("ani-fire-mario-kick"));

	AddAnimation("WarpHor", animations->Get("ani-fire-mario-walk"));
	AddAnimation("WarpVer", animations->Get("ani-fire-mario-idle-front"));
}

void FireMario::MovementAnimation()
{
	// auto animation = GetState(currentState);

	if (attacking)
	{
		if (currentState.compare("Attack") != 0) SetState("Attack");
		// animation->SetSpeedMultiplier(2.0f);
		return;
	}

	CMario::MovementAnimation();
}

void FireMario::OnAnimationEnd()
{
	if (currentState.compare("Attack") == 0)
	{
		attacking = false;
		if (animations.find(lastState) == animations.end())
			lastState = "Idle";
		SetState(lastState);
	}
}

ObjectPool FireMario::GetFireballs()
{
	return this->fireballs;
}
