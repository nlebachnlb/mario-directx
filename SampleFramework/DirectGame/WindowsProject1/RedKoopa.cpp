#include "RedKoopa.h"
#include "Mathf.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "KoopaSpawner.h"
#include "EffectPool.h"

void RedKoopa::Start()
{
	rigidbody->SetGravity(KOOPA_GRAVITY);
	// DebugOut(L"Koopa start: %f, %f\n", colliders->at(0)->GetBoxSize().x, colliders->at(0)->GetBoxSize().x);

	SetState("Walk");
	rigidbody->SetMaterial(KOOPA_PHYSIC_MATERIAL);

	dead = false;
	time = 0;
	facing = direction;
	hit = false;
}

void RedKoopa::Movement()
{
	facing = Mathf::Sign(rigidbody->GetVelocity().x);
	SetScale(Vector2(-facing, transform.Scale.y));
	// DebugOut(L"VEL: %f\n", rigidbody->GetVelocity().x);
}

void RedKoopa::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Walk", animations->Get("ani-red-koopa-troopa-move"));
	AddAnimation("Die", animations->Get("ani-red-koopa-troopa-shell-idle"));
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
		SetState("Die");
		dead = true;
	}
	else
	{
		auto gameMap = Game::GetInstance().GetService<GameMap>();
		auto koopaSpawner = gameMap->GetSpawnerManager()->GetService<KoopaSpawner>();
		auto delta = Vector2(0, KOOPA_BBOX.y - KOOPAS_SHELL_BBOX.y);
		auto shell = koopaSpawner->InstantiateShell(transform.Position + delta * 0.5f, KoopasShellType::Red);
	
		if (hit)
		{
			shell->OnDead(false);
			auto gmap = Game::GetInstance().GetService<GameMap>();
			auto spawner = gmap->GetSpawnerManager();
			auto fxPool = spawner->GetService<EffectPool>();

			fxPool->CreateFX("fx-hit-star", hitPos);
			hit = false;
		}

		time = -1;
		dead = true;
	}
}

void RedKoopa::OnEnabled()
{
	AbstractEnemy::OnEnabled();
	rigidbody->SetVelocity(&Vector2(direction * KOOPA_MOVING_SPEED, 0));
}

void RedKoopa::OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions)
{
	auto selfBox = selfCollider->GetBoundingBox();
	
	for (auto collision : collisions)
	{
		if (collision->collisionDirection.y < 0 == false || collision->collisionDirection.x != 0) continue;
		if (collision->collider->GetGameObject()->GetTag() == ObjectTags::Solid ||
			collision->collider->GetGameObject()->GetTag() == ObjectTags::GhostPlatform || 
			collision->collider->GetGameObject()->GetTag() == ObjectTags::Block)
		{
			auto otherBox = collision->collider->GetBoundingBox();

			if (transform.Position.x > otherBox.right)
			{
				rigidbody->SetVelocity(&Vector2(-Mathf::Abs(rigidbody->GetVelocity().x), rigidbody->GetVelocity().y));
				SetPosition(Vector2(otherBox.right - 1, transform.Position.y));
				// DebugOut(L"BACK: %f\n", rigidbody->GetVelocity().x);
			}
			else if (transform.Position.x < otherBox.left)
			{
				rigidbody->SetVelocity(&Vector2(+Mathf::Abs(rigidbody->GetVelocity().x), rigidbody->GetVelocity().y));
				SetPosition(Vector2(otherBox.left + 1, transform.Position.y));
				// DebugOut(L"BACK-left: %f\n", rigidbody->GetVelocity().x);
			}
		}
	}
}

void RedKoopa::OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider)
{
	if (otherCollider->GetGameObject()->GetTag() == ObjectTags::MarioAttack)
	{
		// DebugOut(L"Koopa crouch\n");
		if (!hit)
		{
			hit = true;
			hitPos = otherCollider->GetGameObject()->GetTransform().Position;
			this->OnDead(false);
		}
		otherCollider->GetGameObject()->SetActive(false);
		otherCollider->GetGameObject()->GetColliders()->at(0)->Disable();
	}
}

Vector2 RedKoopa::GetBoxSize()
{
    return KOOPA_BBOX;
}
