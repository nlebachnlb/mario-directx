#include "BoomerangBrother.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "Mathf.h"
#include "Boomerang.h"

void BoomerangBrother::Awake()
{
	AbstractEnemy::Awake();

	rigidbody->SetDynamic(true);
	rigidbody->SetGravity(BOOMERANG_BRO_GRAVITY);

	visualRelativePosition.y = -6;

	auto boomerang = Instantiate<Boomerang>();
	boomerang->owner = this;
	boomerang->SetPool(&pool);
	pool.Add(boomerang);

	boomerang = Instantiate<Boomerang>();
	boomerang->owner = this;
	boomerang->SetPool(&pool);
	pool.Add(boomerang);
}

void BoomerangBrother::OnEnabled()
{
	AbstractEnemy::OnEnabled();
	UpdateDirection();
	if (!poolRegistered)
	{
		auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
		if (scene != nullptr)
		{
			// DebugOut(L"Venus regis\n");
			pool.RegisterPoolToScene(scene);
			poolRegistered = true;
		}
	}
}

void BoomerangBrother::Start()
{
	SetState("Walk");
	movingPhase = 0;
	throwingPhase = 0;
	timer = timer1 = 0;
}

void BoomerangBrother::Movement()
{
	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	switch (movingPhase)
	{
	case 0:
	{
		auto vel = Vector2(+BOOMERANG_BRO_SPEED, 0);
		rigidbody->SetVelocity(&vel);
		movingPhase = 1;
	}
	break;
	case 1:
	{
		if (transform->Position.x > startPosition.x + BOOMERANG_BRO_MOVEDELTA)
		{
			auto vel = Vector2(0, 0);
			rigidbody->SetVelocity(&vel);
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
				auto vel = Vector2(-BOOMERANG_BRO_SPEED, 0);
				rigidbody->SetVelocity(&vel);
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
			auto vel = Vector2(0, 0);
			rigidbody->SetVelocity(&vel);
		}
	}
	break;
	}

	UpdateDirection();
	transform->Scale.x = -direction;

	switch (throwingPhase)
	{
	case 0:
	{
		timer1 += dt;
		if (timer1 > 4000)
		{
			timer1 = 0;
			ThrowBoomerang();
			throwingPhase = 1;
		}
	}
	break;
	case 1:
	{
		timer1 += dt;
		if (timer1 > 2000)
		{
			timer1 = 0;
			ThrowBoomerang();
			throwingPhase = 2;
		}
	}
	break;
	case 2:
	{
		timer1 += dt;
		if (timer1 > 1000)
		{
			timer1 = 0;
			throwingPhase = 0;
		}
	}
	break;
	}
}

void BoomerangBrother::OnDead(bool oneHit)
{
	Game::GetInstance().GainComboChain(transform->Position);

	movingPhase = 100;
	time = BOOMERANG_BRO_DEAD_TIME * 2;
	colliders->at(0)->Disable();
	transform->Scale.y = -1;
	auto vel = Vector2(-rigidbody->GetVelocity().x, oneHit ? BOOMERANG_BRO_DEFLECTION_ON_SHOT : 0);
	rigidbody->SetVelocity(&vel);
	dead = true;
}

void BoomerangBrother::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Walk", animations->Get("ani-boomerang-brother-move"));
	AddAnimation("Attack", animations->Get("ani-boomerang-brother-attack"));
}

void BoomerangBrother::OnOverlapped(Collider2D* self, Collider2D* other)
{
	if (other->GetGameObject()->GetTag() == ObjectTags::MarioAttack)
	{
		//DebugOut(L"Goomba die\n");
		this->OnDead(true);
		other->GetGameObject()->SetActive(false);
		other->GetGameObject()->GetColliders()->at(0)->Disable();
	}
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

void BoomerangBrother::ThrowBoomerang()
{
	auto obj = pool.Instantiate();

	if (obj != nullptr)
	{
		auto boomerang = static_cast<Boomerang*>(obj);
		boomerang->facing = direction;
		boomerang->SetPosition(transform->Position - Vector2(0, 16));
		boomerang->Start();
	}
}
