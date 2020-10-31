#include "RedKoopa.h"
#include "Mathf.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "KoopaSpawner.h"

void RedKoopa::Start()
{
	int direction = Random::Range(0, 100) < 50 ? 1 : -1;
	rigidbody->SetVelocity(&Vector2(direction * KOOPA_MOVING_SPEED, 0));
	rigidbody->SetGravity(KOOPA_GRAVITY);
	DebugOut(L"Koopa start: %f, %f\n", colliders->at(0)->GetBoxSize().x, colliders->at(0)->GetBoxSize().x);

	SetState("Walk");
	rigidbody->SetMaterial(KOOPA_PHYSIC_MATERIAL);

	dead = false;
	time = 0;
	facing = direction;
}

void RedKoopa::Movement()
{
	facing = Mathf::Sign(rigidbody->GetVelocity().x);
	SetScale(Vector2(-facing, transform.Scale.y));
}

void RedKoopa::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Walk", animations->Clone("ani-red-koopa-troopa-move"));
	AddAnimation("Die", animations->Clone("ani-red-koopa-troopa-shell-idle"));
}

void RedKoopa::OnDead(bool oneHit)
{
	if (oneHit)
	{
		rigidbody->SetGravity(KOOPA_GRAVITY);
		time = KOOPA_DEAD_TIME * 2;
		colliders->at(0)->Disable();
		transform.Scale.y = -1;
		rigidbody->SetVelocity(&Vector2(-rigidbody->GetVelocity().x, KOOPA_DEFLECTION_ON_SHOT));
		SetState("Idle");
		dead = true;
	}
	else
	{
		auto gameMap = Game::GetInstance().GetService<GameMap>();
		auto koopaSpawner = gameMap->GetSpawnerManager()->GetService<KoopaSpawner>();
		koopaSpawner->InstantiateShell(transform.Position);
		time = -1;
		dead = true;
	}
}

void RedKoopa::OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider)
{
	if (otherCollider->GetGameObject()->GetTag() == ObjectTags::MarioAttack)
	{
		DebugOut(L"Koopa crouch\n");
		this->OnDead(false);
		otherCollider->GetGameObject()->SetActive(false);
		otherCollider->GetGameObject()->GetColliders()->at(0)->Disable();
	}
}

Vector2 RedKoopa::GetBoxSize()
{
    return KOOPA_BBOX;
}
