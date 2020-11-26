#include "RedParagoomba.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "GoombaSpawner.h"
#include "Mathf.h"

void RedParagoomba::Start()
{
	Goomba::Start();
	SetState("Walk");
	timer = 0;
	jumpStep = 0;
}

void RedParagoomba::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Idle", animations->Clone("ani-red-para-goomba-idle"));
	AddAnimation("Walk", animations->Clone("ani-red-para-goomba-walk"));
	AddAnimation("Fly", animations->Clone("ani-red-para-goomba-fly"));
}

void RedParagoomba::Movement()
{
	if (onGround)
	{
		switch (jumpStep)
		{
		case 0:
		{
			UpdateDirection();
			auto curVel = rigidbody->GetVelocity();
			curVel.x = Mathf::Abs(curVel.x) * direction;
			rigidbody->SetVelocity(&curVel);
			timer = 0;
			jumpStep = 1;
		}
		break;
		case 1:
		{
			timer += Game::DeltaTime() * Game::GetTimeScale();
			if (timer >= RED_PARAGOOMBA_BREAK_TIME)
			{
				timer = 0;
				jumpStep = 2;
			}
		}
		break;
		case 2:
		case 3:
		case 4:
		{
			auto curVel = rigidbody->GetVelocity();
			curVel.y = -RED_PARAGOOMBA_LOW_JUMP;
			rigidbody->SetVelocity(&curVel);
			onGround = false;
			jumpStep++;
		}
		break;
		case 5:
		{
			auto curVel = rigidbody->GetVelocity();
			curVel.y = -RED_PARAGOOMBA_HIGH_JUMP;
			rigidbody->SetVelocity(&curVel);
			onGround = false;
			jumpStep = 6;
		}
		break;
		case 6:
			jumpStep = 0;
			break;
		}
	}
}

void RedParagoomba::PreRender()
{
	if (dead) return;

	if (jumpStep >= 2 && currentState.compare("Fly") != 0)
		SetState("Fly");
	else if (jumpStep < 2 && currentState.compare("Walk") != 0)
		SetState("Walk");
}

void RedParagoomba::OnDead(bool oneHit)
{
	if (oneHit)
	{
		time = GOOMBA_DEAD_TIME * 2;
		colliders->at(0)->Disable();
		transform.Scale.y = -1;
		rigidbody->SetVelocity(&Vector2(-0 * rigidbody->GetVelocity().x, GOOMBA_DEFLECTION_ON_SHOT));
		SetState("Idle");
		dead = true;
	}
	else
	{
		auto gameMap = Game::GetInstance().GetService<GameMap>();
		auto goombaSpawner = gameMap->GetSpawnerManager()->GetService<GoombaSpawner>();

		auto goomba = goombaSpawner->Spawn("enm-red-goomba", transform.Position);
		rigidbody->SetVelocity(&Vector2(rigidbody->GetVelocity().x, 0));
		rigidbody->PassDataTo(goomba->GetRigidbody());
		time = -1;
		dead = true;
	}
}

void RedParagoomba::OnCollisionEnter(Collider2D* self, std::vector<CollisionEvent*> collisions)
{
	for (auto col : collisions)
	{
		if (col->collisionDirection.y < 0 && TagUtils::PlatformTag(col->collider->GetGameObject()->GetTag()) && !onGround)
			onGround = true;
	}
}
