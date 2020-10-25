#include "RaccoonMario.h"
#include "Game.h"
#include "AnimationDatabase.h"

void RaccoonMario::Awake()
{
	CMario::Awake();
}

void RaccoonMario::Start()
{
	CMario::Start();

	attacking = false;
	floating = false;
}

void RaccoonMario::OnAnimationEnd()
{
	if (currentState.compare("Attack") == 0 || 
		currentState.compare("Float") == 0)
	{
		attacking = false;
		if (animations.find(lastState) == animations.end())
			lastState = "Idle";
		SetState(lastState);
	}
}

void RaccoonMario::OnKeyDown(int keyCode)
{
	CMario::OnKeyDown(keyCode);

	// Process attack key
	if (keyCode == marioKeySet.Attack && attacking == false)
	{
		attacking = true;
	}

	// Mario uses his tail to keep him floating on the air
	if (keyCode == marioKeySet.Jump && onGround == false)
	{
		if (physicState.jump == JumpingStates::Fall)
		{
			floating = true;
			lastFloatingTime = GetTickCount();
		}
	}
}

void RaccoonMario::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();

	AddAnimation("Run", animations->Get("ani-raccoon-mario-speed-up"));
	AddAnimation("Walk", animations->Get("ani-raccoon-mario-walk"));
	AddAnimation("Jump", animations->Get("ani-raccoon-mario-jump"));
	AddAnimation("Fly", animations->Get("ani-raccoon-mario-jump"));
	AddAnimation("Float", animations->Get("ani-raccoon-mario-float"), false);
	AddAnimation("FullFly", animations->Get("ani-raccoon-mario-fly"));
	AddAnimation("Fall", animations->Get("ani-raccoon-mario-fall"));
	AddAnimation("Idle", animations->Get("ani-raccoon-mario-idle"));
	AddAnimation("Skid", animations->Get("ani-raccoon-mario-skid"));
	AddAnimation("Crouch", animations->Get("ani-raccoon-mario-crouch"));
	AddAnimation("Attack", animations->Get("ani-raccoon-mario-spin"), false);
}

void RaccoonMario::MovementAnimation()
{
	auto animation = GetState(currentState);

	if (attacking)
	{
		if (currentState.compare("Attack") != 0) SetState("Attack");
		animation->SetSpeedMultiplier(2.0f);
		return;
	}

	CMario::MovementAnimation();
}

void RaccoonMario::JumpingAnimation()
{
	if (floating)
	{
		if (currentState.compare("Float") != 0) SetState("Float");
		// DebugOut(L"[FLOAT] floating\n");
		return;
	}

	DebugOut(L"[JUMP] jump\n");
	CMario::JumpingAnimation();
}

void RaccoonMario::LateUpdate()
{
	if (floating)
	{
		auto current = GetTickCount();
		if (current - lastFloatingTime > RACCOON_FLOAT_TIME)
		{
			floating = false;
			this->rigidbody->SetGravity(MARIO_GRAVITY);
		}
		else
		{
			this->rigidbody->SetGravity(MARIO_GRAVITY * 0.01f);
		}
	}
}
