#include "Goomba.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "Mathf.h"

void Goomba::Start()
{
	int direction = Random::Range(0, 100) < 50 ? 1 : -1;
	rigidbody->SetVelocity(&Vector2(direction * GOOMBA_MOVING_SPEED, 0));
	rigidbody->SetGravity(GOOMBA_GRAVITY);
	DebugOut(L"Goomba start: %f, %f\n", colliders->at(0)->GetBoxSize().x, colliders->at(0)->GetBoxSize().x);

	SetState("Walk");
	rigidbody->SetMaterial(GOOMBA_PHYSIC_MATERIAL);
}

void Goomba::Movement()
{

}

void Goomba::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Walk", animations->Clone("ani-goomba-walk"));
}

void Goomba::OnDead()
{
	
}

void Goomba::OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions)
{
	/*for (auto collision : collisions)
	{
		if (collision->collisionDirection.x != 0)
		{
			rigidbody->SetVelocity(&Vector2(Mathf::Sign(collision->collisionDirection.x) * GOOMBA_MOVING_SPEED, rigidbody->GetVelocity().y));
			DebugOut(L"Goomba vel: %f\n", rigidbody->GetVelocity().x);
			return;
		}
	}*/
}

Vector2 Goomba::GetBoxSize()
{
	return GOOMBA_BBOX;
}
