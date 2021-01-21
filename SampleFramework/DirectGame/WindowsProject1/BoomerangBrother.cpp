#include "BoomerangBrother.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "Mathf.h"

void BoomerangBrother::Awake()
{
	AbstractEnemy::Awake();

	rigidbody->SetDynamic(true);
	rigidbody->SetGravity(BOOMERANG_BRO_GRAVITY);

	visualRelativePosition.y = -6;
}

void BoomerangBrother::Start()
{
	SetState("Walk");
	movingPhase = 0;
	timer = 0;
}

void BoomerangBrother::Movement()
{
	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	switch (movingPhase)
	{
	case 0:
	{
		rigidbody->SetVelocity(&Vector2(+BOOMERANG_BRO_SPEED, 0));
		movingPhase = 1;
	}
	break;
	case 1:
	{
		if (transform->Position.x > startPosition.x + BOOMERANG_BRO_MOVEDELTA)
		{
			rigidbody->SetVelocity(&Vector2(0, 0));
			movingPhase = 2;
		}

		if (rigidbody->GetVelocity().x < 0)
			movingPhase = 3;
	}
	break;
	case 2:
	case 4:
	{
		timer += dt;
		if (timer > BOOMERANG_BRO_IDLE_TIME)
		{
			timer = 0;
			if (movingPhase == 2)
			{
				movingPhase = 3;
				rigidbody->SetVelocity(&Vector2(-BOOMERANG_BRO_SPEED, 0));
			}
			else if (movingPhase == 4) movingPhase = 0;
		}
	}
	break;
	case 3:
	{
		if (transform->Position.x < startPosition.x)
		{
			timer = 0;
			movingPhase = 4;
			rigidbody->SetVelocity(&Vector2(0, 0));
		}
	}
	break;
	}

	UpdateDirection();
	transform->Scale.x = -direction;
}

void BoomerangBrother::OnDead(bool oneHit)
{
	Game::GetInstance().GainComboChain(transform->Position);

	movingPhase = 100;
	time = BOOMERANG_BRO_DEAD_TIME * 2;
	colliders->at(0)->Disable();
	transform->Scale.y = -1;
	rigidbody->SetVelocity(&Vector2(-rigidbody->GetVelocity().x, oneHit ? BOOMERANG_BRO_DEFLECTION_ON_SHOT : 0));
	dead = true;
}

void BoomerangBrother::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Walk", animations->Get("ani-boomerang-brother-move"));
	AddAnimation("Attack", animations->Get("ani-boomerang-brother-attack"));
}

Vector2 BoomerangBrother::GetBoxSize()
{
	return BOOMERANG_BRO_BBOX;
}

void BoomerangBrother::UpdateDirection()
{
	if (player == nullptr) player = Game::GetInstance().FindGameObjectWithTag(ObjectTags::Player, true);
	direction = player == nullptr ? -1 : Mathf::Sign(player->GetTransform().Position.x - transform->Position.x);
}
