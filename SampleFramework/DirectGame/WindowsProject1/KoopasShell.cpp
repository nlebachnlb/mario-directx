#include "KoopasShell.h"
#include "QuestionBlock.h"
#include "CMario.h"
#include "Game.h"
#include "EffectPool.h"
#include "Mathf.h"

void KoopasShell::Awake()
{
	AbstractEnemy::Awake();
	SetTag(ObjectTags::KoopasShell);
	renderOrder = 5;
	rigidbody->SetDrag(Vector2(KOOPAS_SHELL_DRAG_FORCE, 0));
}

void KoopasShell::Start()
{
	rigidbody->SetGravity(KOOPAS_SHELL_GRAVITY);
	DebugOut(L"Koopa shell start: %f, %f\n", colliders->at(0)->GetBoxSize().x, colliders->at(0)->GetBoxSize().x);

	SetState("Idle");
	rigidbody->SetMaterial(KOOPAS_SHELL_PHYSIC_MATERIAL);

	dead = false;
	time = 0;
	Release();
	StopRunning();
	running = false;

	transform->Scale.y = 1;
	timer = 0;
	withdrawStep = 0;
}

void KoopasShell::Movement()
{
	rigidbody->SetGravity(KOOPAS_SHELL_GRAVITY * (IsHeld() ? 0 : 1));
	rigidbody->SetVelocity(IsHeld() ? &VectorZero() : &rigidbody->GetVelocity());
	// DebugOut(L"Shell vel: %f\n", rigidbody->GetVelocity().y);

	if (!running) rigidbody->GenerateDragForce();
	else if (rigidbody->GetVelocity().x == 0)
		rigidbody->SetVelocity(&Vector2(facing * KOOPAS_SHELL_MOVING_SPEED, rigidbody->GetVelocity().y));

	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	switch (withdrawStep)
	{
	case 0:
	{
		timer += dt;
		if (timer > KOOPAS_WITHDRAW_TIME)
		{
			timer = 0;
			StartWithdrawing();
		}
	}
	break;
	case 1:
	{
		auto a = WITHDRAW_AMPLITUDE;
		auto t = WITHDRAW_ROUNDTRIP_TIME;
		auto omega = 2 * Mathf::Pi / (float)t;
		auto phi = Mathf::Pi;
		auto movement = Mathf::Cos(omega * timer + phi);

		visualRelativePosition.x = a * movement;
		timer += dt;

		// animations.at(currentState)->SetSpeedMultiplier()
		if (timer > 1300)
		{
			timer = 0;
			withdrawStep = 2;
			visualRelativePosition.x = 0;
			OnWithdraw();

			if (IsHeld())
			{
				auto holderTag = holder->GetTag();
				if (TagUtils::MarioTag(holderTag))
				{
					auto mario = static_cast<CMario*>(holder);
					mario->ReleaseInHandObject();
					Release();
					StopRunning();
					mario->OnDamaged(this);
				}
			}

			linkedPool->Revoke(this);
		}
	}
	break;
	}
}

void KoopasShell::OnDead(bool oneHit)
{
	if (oneHit)
	{
		rigidbody->SetGravity(KOOPAS_SHELL_GRAVITY);
		time = KOOPAS_SHELL_DEAD_TIME * 2;
		colliders->at(0)->Disable();
		transform->Scale.y = -1;
		StopRunning();
		rigidbody->SetVelocity(&Vector2(-facing * KOOPAS_SHELL_HITBACK_SPEED, KOOPAS_SHELL_DEFLECTION_ON_SHOT));
		SetState("Idle");
		dead = true;
		Game::GetInstance().GainComboChain(transform->Position);
	}
	else
	{
		rigidbody->SetGravity(KOOPAS_SHELL_GRAVITY);
		transform->Scale.y = -1;
		StopRunning();
		rigidbody->SetVelocity(&Vector2(facing * KOOPAS_SHELL_HITBACK_SPEED, KOOPAS_SHELL_DEFLECTION_ON_SHOT));
		SetState("Idle");
	}
}

void KoopasShell::OnCollisionEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions)
{
	for (auto collision : collisions)
	{
		auto otherTag = collision->collider->GetGameObject()->GetTag();

		if (otherTag == ObjectTags::Block && running)
		{
			if (collision->collisionDirection.y != 0) continue;
			auto block = static_cast<AbstractBlock*>(collision->collider->GetGameObject());
			block->Bounce(this);
		}
	}
}

void KoopasShell::OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider)
{
	if (otherCollider->GetGameObject()->GetTag() == ObjectTags::Solid)
	{
		// DebugOut(L"Overlapp solid\n"); 
		if (running)
			this->OnDead(true);
	}

	if (TagUtils::EnemyTag(otherCollider->GetGameObject()->GetTag()) && dead == false)
	{
		auto enemy = static_cast<AbstractEnemy*>(otherCollider->GetGameObject());
		DebugOut(L"Enemy OVERLAP\n");
		auto gmap = Game::GetInstance().GetService<GameMap>();
		auto spawner = gmap->GetSpawnerManager();
		auto fxPool = spawner->GetService<EffectPool>();

		if (running || IsHeld())
		{
			if (enemy->IsDead() == false)
			{
				Vector2 pos = transform->Position;
				fxPool->CreateFX("fx-hit-star", pos);
				enemy->OnDead(true);
			}

			if (running && tag == enemy->GetTag()) OnDead(true);
			else if (IsHeld())
			{
				auto holderTag = holder->GetTag();
				if (TagUtils::MarioTag(holderTag))
				{
					static_cast<CMario*>(holder)->ReleaseInHandObject();
					Release();
					StopRunning();
					OnDead(true);
				}
			}
		}
	}
}

void KoopasShell::OnOffScreen()
{
	time = -1;
	dead = true;
	linkedPool->Revoke(this);
}

Vector2 KoopasShell::GetColliderBox()
{
	return colliders->at(0)->GetBoxSize();
}

void KoopasShell::SetHoldablePosition(Vector2 position)
{
	SetPosition(position);
}

void KoopasShell::OnRelease()
{
	// colliders->at(0)->SetTrigger(false);
	colliders->at(0)->Enable();
	SetFacing(holdableFacing);
	Run();
}

void KoopasShell::SetFacing(int facing)
{
	this->facing = facing;
}

void KoopasShell::Run()
{
	if (!running)
	{
		rigidbody->SetGravity(KOOPAS_SHELL_GRAVITY);
		rigidbody->SetVelocity(&Vector2(facing * KOOPAS_SHELL_MOVING_SPEED, rigidbody->GetVelocity().y));
		SetState("Run");
		running = true;
		withdrawStep = 2;
	}
}

void KoopasShell::StopRunning()
{
	if (running)
	{
		rigidbody->SetGravity(KOOPAS_SHELL_GRAVITY);
		rigidbody->SetVelocity(&Vector2(0, rigidbody->GetVelocity().y));
		SetState("Idle");
		running = false;
		withdrawStep = 0;
		timer = 0;
	}
}

bool KoopasShell::IsRunning()
{
	return this->running;
}

Vector2 KoopasShell::GetBoxSize()
{
	return KOOPAS_SHELL_BBOX;
}

void KoopasShell::StartWithdrawing()
{
	SetState("Withdraw");
	withdrawStep = 1;
	timer = 0;
}
