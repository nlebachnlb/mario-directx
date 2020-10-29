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
	// DebugOut(L"Shell position: %f, %f\n", transform.Position.x, transform.Position.y);
}

void KoopasShell::OnDead(bool oneHit)
{
	if (oneHit)
	{
		time = KOOPAS_SHELL_DEAD_TIME * 2;
		colliders->at(0)->Disable();
		transform.Scale.y = -1;
		rigidbody->SetVelocity(&Vector2(0, KOOPAS_SHELL_DEFLECTION_ON_SHOT));
		SetState("Idle");
		dead = true;
	}
	else
	{
		
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
	SetFacing(holdableFacing);
	Run();
	colliders->at(0)->Enable();
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
