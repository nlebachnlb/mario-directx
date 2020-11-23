#include "HitFX.h"

void HitFX::Awake()
{
	InitAnimations();
	rigidbody->SetDynamic(false);
}

void HitFX::Start()
{
	SetState("Default");
	renderOrder = 11;
}

void HitFX::OnAnimationEnd()
{
	pool->Revoke(this);
}
