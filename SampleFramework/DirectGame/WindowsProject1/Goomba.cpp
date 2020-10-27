#include "Goomba.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "Mathf.h"

void Goomba::Start()
{
	int direction = Random::Range(0, 100) < 50 ? 1 : -1;
	rigidbody->SetVelocity(&Vector2(direction * GOOMBA_MOVING_SPEED, 0));
	rigidbody->SetGravity(GOOMBA_GRAVITY);
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
	rigidbody->SetVelocity(&Vector2(-1 * rigidbody->GetVelocity().x, rigidbody->GetVelocity().y));
}

Vector2 Goomba::GetBoxSize()
{
	return GOOMBA_BBOX;
}
