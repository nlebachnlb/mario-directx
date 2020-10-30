#include "KoopasShell.h"

void KoopasShell::Awake()
{
	AbstractEnemy::Awake();
	SetTag(ObjectTags::KoopasShell);
	renderOrder = 10;
}

void KoopasShell::Start()
{
	rigidbody->SetGravity(KOOPAS_SHELL_GRAVITY);
	DebugOut(L"Koopa shell start: %f, %f\n", colliders->at(0)->GetBoxSize().x, colliders->at(0)->GetBoxSize().x);

	SetState("Idle");
	rigidbody->SetMaterial(KOOPAS_SHELL_PHYSIC_MATERIAL);

	dead = false;
	time = 0;
	running = false;
}

void KoopasShell::Movement()
{
	rigidbody->SetGravity(KOOPAS_SHELL_GRAVITY * (IsHeld() ? 0 : 1));
	// DebugOut(L"Shell vel: %f\n", rigidbody->GetVelocity().y);
}

void KoopasShell::OnDead(bool oneHit)
{
	if (oneHit)
	{
		rigidbody->SetGravity(KOOPAS_SHELL_GRAVITY);
		time = KOOPAS_SHELL_DEAD_TIME * 2;
		colliders->at(0)->Disable();
		transform.Scale.y = -1;
		rigidbody->SetVelocity(&Vector2(0, KOOPAS_SHELL_DEFLECTION_ON_SHOT));
		SetState("Idle");
		dead = true;
	}
	else
	{
		rigidbody->SetGravity(KOOPAS_SHELL_GRAVITY);
		transform.Scale.y = -1;
		StopRunning();
		rigidbody->SetVelocity(&Vector2(0, KOOPAS_SHELL_DEFLECTION_ON_SHOT));
		SetState("Idle");
	}
}

void KoopasShell::OnCollisionEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions)
{
	for (auto collision : collisions)
	{
		auto otherTag = collision->collider->GetGameObject()->GetTag();
		if (TagUtils::EnemyTag(otherTag))
		{
			auto enemy = static_cast<AbstractEnemy*>(collision->collider->GetGameObject());
			if (running)
				enemy->OnDead(true);
		}

		/*if (otherTag == ObjectTags::MarioAttack)
		{
			this->OnDead(false);
			collision->collider->GetGameObject()->SetActive(false);
			collision->collider->GetGameObject()->GetColliders()->at(0)->Disable();
		}

		if (otherTag == ObjectTags::FriendlyProjectiles)
			this->OnDead(true);*/
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

	if (otherCollider->GetGameObject()->GetTag() == ObjectTags::MarioAttack)
	{
		DebugOut(L"Overlapp atta mario\n");
		this->OnDead(false);
		otherCollider->GetGameObject()->SetActive(false);
		otherCollider->GetGameObject()->GetColliders()->at(0)->Disable();
	}
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
		rigidbody->SetVelocity(&Vector2(facing * KOOPAS_SHELL_MOVING_SPEED, 0));
		SetState("Run");
		running = true;
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
