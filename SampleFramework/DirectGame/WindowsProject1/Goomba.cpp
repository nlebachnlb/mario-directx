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

	dead = false;
	time = 0;
}

void Goomba::Movement()
{

}

void Goomba::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Idle", animations->Clone("ani-goomba-idle"));
	AddAnimation("Walk", animations->Clone("ani-goomba-walk"));
	AddAnimation("Die", animations->Clone("ani-goomba-die"));
}

void Goomba::OnDead(bool oneHit)
{
	if (oneHit)
	{
		time = GOOMBA_DEAD_TIME * 2;
		colliders->at(0)->Disable();
		transform.Scale.y = -1;
		rigidbody->SetVelocity(&Vector2(0, GOOMBA_DEFLECTION_ON_SHOT));
		SetState("Idle");
		dead = true;
	}
	else
	{
		time = GOOMBA_DEAD_TIME;
		rigidbody->SetDynamic(false);
		colliders->at(0)->Disable();
		SetVisualRelativePosition(Vector2(0, 7 * 1.5f));
		SetState("Die");
		dead = true;
	}
}

void Goomba::OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions)
{
	for (auto collision : collisions)
	{
		if (collision->collider->GetGameObject()->GetTag() == ObjectTags::MarioAttack)
		{
			DebugOut(L"Goomba die\n");
			this->OnDead(true);
		}
	}
}

Vector2 Goomba::GetBoxSize()
{
	return GOOMBA_BBOX;
}
