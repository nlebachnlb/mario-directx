#include "Goomba.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "Mathf.h"

void Goomba::Start()
{
	rigidbody->SetGravity(GOOMBA_GRAVITY);

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
	AddAnimation("Idle", animations->Get("ani-goomba-idle"));
	AddAnimation("Walk", animations->Get("ani-goomba-walk"));
	AddAnimation("Die", animations->Get("ani-goomba-die"));
}

void Goomba::OnDead(bool oneHit)
{
	Game::GetInstance().GainComboChain(transform->Position);
	if (oneHit)
	{
		time = GOOMBA_DEAD_TIME * 2;
		colliders->at(0)->Disable();
		transform->Scale.y = -1;
		rigidbody->SetVelocity(&Vector2(-1 * rigidbody->GetVelocity().x, GOOMBA_DEFLECTION_ON_SHOT));
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

void Goomba::OnEnabled()
{
	AbstractEnemy::OnEnabled();
	rigidbody->SetVelocity(&Vector2(direction * GOOMBA_MOVING_SPEED, 0));
}

void Goomba::OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider)
{
	if (otherCollider->GetGameObject()->GetTag() == ObjectTags::MarioAttack)
	{
		DebugOut(L"Goomba die\n");
		this->OnDead(true);
		otherCollider->GetGameObject()->SetActive(false);
		otherCollider->GetGameObject()->GetColliders()->at(0)->Disable();
	}
}

Vector2 Goomba::GetBoxSize()
{
	return GOOMBA_BBOX;
}
