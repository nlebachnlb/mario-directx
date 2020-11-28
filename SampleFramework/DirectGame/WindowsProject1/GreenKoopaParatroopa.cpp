#include "GreenKoopaParatroopa.h"
#include "Game.h"
#include "AnimationDatabase.h"

void GreenKoopaParatroopa::Start()
{
	SetState("Fly");
	onGround = false;
	troopa = Troopa::Green;
	rigidbody->SetGravity(KOOPA_PARATROOPA_GRAVITY);
}

void GreenKoopaParatroopa::Movement()
{
	if (onGround)
	{
		auto curVel = rigidbody->GetVelocity(); curVel.y = -KOOPA_PARATROOPA_JUMP_VEL;
		rigidbody->SetVelocity(&curVel);
		onGround = false;
	}
}

void GreenKoopaParatroopa::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Fly", animations->Get("ani-green-koopa-paratroopa-fly"));
	AddAnimation("Idle", animations->Get("ani-green-koopa-paratroopa-idle"));
	AddAnimation("Die", animations->Get("ani-green-koopa-troopa-shell-idle"));
}

void GreenKoopaParatroopa::OnCollisionEnter(Collider2D* self, std::vector<CollisionEvent*> collisions)
{
	for (auto col : collisions)
	{
		if (col->collisionDirection.y < 0 && TagUtils::PlatformTag(col->collider->GetGameObject()->GetTag()) && !onGround)
			onGround = true;
	}
}
