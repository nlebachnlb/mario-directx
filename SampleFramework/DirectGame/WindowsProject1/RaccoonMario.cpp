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
	flying = 0;
	pushing = false;
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

	if (keyCode == marioKeySet.Jump)
	{
		if (pMeter >= PMETER_MAX && flying == 0)
		{
			DebugOut(L"[fly] start\n");
			flying = 1;
			lastFlyingTime = GetTickCount();
		}
	}

	// Mario uses his tail to keep him floating on the air
	if (keyCode == marioKeySet.Jump && onGround == false)
	{
		if (flying == 1 && (physicState.jump == JumpingStates::High))
		{
			pushing = true;
			lastPushingTime = GetTickCount();
		}

		if (physicState.jump == JumpingStates::Fall)
		{
			if (flying == 1)
			{
				pushing = true;
				lastPushingTime = GetTickCount();
			}
			else
			{
				floating = true;
				lastFloatingTime = GetTickCount();
			}
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
	if (flying == 1)
	{
		if (currentState.compare("FullFly") != 0) SetState("FullFly");
		return;
	}

	if (floating)
	{
		if (currentState.compare("Float") != 0) SetState("Float");
		return;
	}

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


	if (flying == 1 && GetTickCount() - lastFlyingTime > RACCOON_FLY_TIME)
	{
		flying = 2;
		pMeter = 0;
		rigidbody->SetGravity(MARIO_GRAVITY);
		DebugOut(L"[fly] done\n");
	}

	if (flying == 1)
	{
		pMeter = PMETER_MAX;

		if (physicState.jump == JumpingStates::Jump || physicState.jump == JumpingStates::High)
		{
			if (pushing && !(GetTickCount() - lastPushingTime > RACCOON_FLOAT_TIME))
			{
				rigidbody->SetGravity(0.0f);
				rigidbody->SetVelocity(&Vector2(rigidbody->GetVelocity().x * 0.95f, RACCOON_FLY_VELOCITY));
			}
			else
			{
				this->rigidbody->SetGravity(MARIO_GRAVITY);
				pushing = false;
			}
		}
	}
	else if (flying == 2)
	{
		if (onGround) flying = 0;
		else pMeter = 0;
	}
}
