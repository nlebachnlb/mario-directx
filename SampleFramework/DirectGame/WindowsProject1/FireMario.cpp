#include "FireMario.h"
#include "AnimationDatabase.h"
#include "Game.h"

void FireMario::Awake()
{
	CMario::Awake();

	SetTag(ObjectTags::PowerupMario);
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
	if (keyCode == marioKeySet.Attack && attacking == false)
	{
		attacking = true;
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
