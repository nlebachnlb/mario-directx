#include "RaccoonMario.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "AbstractEnemy.h"

void RaccoonMario::Awake()
{
	CMario::Awake();
	attackBox = Instantiate<RaccoonAttackBox>();
}

void RaccoonMario::Start()
{
	CMario::Start();
	SetTag(ObjectTags::PowerupMario);
	attacking = false;
	floating = false;
	flying = 0;
	pushing = false;
	feverState = 0;
	feverTime = RACCOON_FEVER_TIME;
	physicalAttacking = 0;
	attackBox->GetColliders()->at(0)->Disable();
	attackBox->SetActive(false);
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
	if (keyCode == marioKeySet.Attack && 
		attacking == false && physicalAttacking == 0 &&
		flying == 0 && physicState.movement != MovingStates::Crouch)
	{
		physicalAttacking = 1;
		lastAttackingTime = GetTickCount();
		attacking = true;
	}

	if (keyCode == marioKeySet.Jump)
	{
		if (feverState == 2 && flying == 0)
		{
			DebugOut(L"[fly] start\n");
			mainCamera->FreeBoundary();
			Jump();
			flying = 1;
			lastFlyingTime = GetTickCount();
		}
		else if (flying == 1)
			mainCamera->FreeBoundary();
	}

	// Mario uses his tail to keep him floating on the air
	if (keyCode == marioKeySet.Jump && onGround == false)
	{
		if (flying == 1 && (physicState.jump == JumpingStates::High))
			pushing = true, lastPushingTime = GetTickCount();

		if (physicState.jump == JumpingStates::Fall)
		{
			if (flying == 1)
				pushing = true, lastPushingTime = GetTickCount();
			else
				floating = true, lastFloatingTime = GetTickCount();
		}
	}
}

void RaccoonMario::Exit()
{
	attackBox->GetColliders()->at(0)->Disable();
	attackBox->SetActive(false);
	flying = 0;
	rigidbody->SetGravity(MARIO_GRAVITY);
	CMario::Exit();
}

void RaccoonMario::RegisterToScene(Scene* scene)
{
	scene->AddObject(attackBox);
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

	AddAnimation("HoldIdle", animations->Get("ani-raccoon-mario-hold-idle"));
	AddAnimation("HoldMove", animations->Get("ani-raccoon-mario-hold"));
	AddAnimation("HoldJump", animations->Get("ani-raccoon-mario-hold-jump"));
	AddAnimation("Kick", animations->Get("ani-raccoon-mario-kick"));

	AddAnimation("WarpHor", animations->Get("ani-raccoon-mario-walk"));
	AddAnimation("WarpVer", animations->Get("ani-raccoon-mario-idle-front"));
}

void RaccoonMario::MovementAnimation()
{
	if (attacking)
	{
		if (currentState.compare("Attack") != 0) SetState("Attack");
		return;
	}

	CMario::MovementAnimation();
}

void RaccoonMario::JumpingAnimation()
{
	if (flying == 1 && !hold)
	{
		if (currentState.compare("FullFly") != 0) SetState("FullFly");
		return;
	}

	if (floating && !hold)
	{
		if (currentState.compare("Float") != 0) SetState("Float");
		return;
	}

	if (attacking)
	{
		if (currentState.compare("Attack") != 0) SetState("Attack");
		return;
	}

	CMario::JumpingAnimation();
}

void RaccoonMario::LateUpdate()
{
	if (warp) return;

	if (floating)
	{
		auto current = GetTickCount();
		if (current - lastFloatingTime > RACCOON_FLOAT_TIME)
			floating = false, this->rigidbody->SetGravity(MARIO_GRAVITY);
		else
			this->rigidbody->SetGravity(0.0f);
	}

	if (flying == 1 && GetTickCount() - lastFlyingTime > RACCOON_FLY_TIME)
	{
		flying = 2;
		pMeter = 0;
		rigidbody->SetGravity(MARIO_GRAVITY);
		// DebugOut(L"[fly] done\n");
	}

	if (flying == 1)
	{
		pMeter = PMETER_MAX;

		if (physicState.jump == JumpingStates::Jump || physicState.jump == JumpingStates::High)
		{
			if (pushing && !(GetTickCount() - lastPushingTime > RACCOON_FLOAT_TIME))
			{
				rigidbody->SetGravity(0.0f);
				rigidbody->SetVelocity(&Vector2(rigidbody->GetVelocity().x, RACCOON_FLY_VELOCITY));
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
		if (onGround)
		{
			flying = 0;
			pMeter = 0;
		}
		else 
			pMeter = 0;
	}

	runningRestriction = flying != 0;

	if (physicalAttacking)
	{
		if (GetTickCount() - lastAttackingTime > 0.5f * RACCOON_ATTACK_TIME && physicalAttacking == 1)
		{
			physicalAttacking = 2;
			attackBox->SetActive(true);
			attackBox->GetColliders()->at(0)->Enable();
		}

		if (GetTickCount() - lastAttackingTime > RACCOON_ATTACK_TIME)
		{
			physicalAttacking = 0;
			attackBox->SetActive(false);
			attackBox->GetColliders()->at(0)->Disable();
		}
	}
 
	attackBox->SetPosition(transform.Position + 
		Vector2((MARIO_BBOX.x + RACCOON_ATTACK_BOX.x) * 0.5f * facing, RACCOON_ATTACK_BOX.y * 0.4f));
}

void RaccoonMario::OnCollisionEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions)
{
	CMario::OnCollisionEnter(selfCollider, collisions);
}
