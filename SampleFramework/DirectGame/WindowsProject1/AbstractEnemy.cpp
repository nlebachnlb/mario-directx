#include "AbstractEnemy.h"
#include "Game.h"
#include "Mathf.h"
#include "PhysicConstants.h"

void AbstractEnemy::Awake()
{
	SetTag(ObjectTags::Enemy);

	InitAnimations();

	Collider2D* box = new Collider2D();
	box->SetBoxSize(GetBoxSize());
	box->AttachToEntity(this);
	this->colliders->push_back(box);

	rigidbody->SetDynamic(true);
	rigidbody->SetFallLimitVelocity(DEFAULT_FALL_LIMIT_VEL);
}

void AbstractEnemy::Update()
{
	Movement();
}

void AbstractEnemy::LateUpdate()
{
	if (dead)
	{
		if (time > 0)
			time -= Game::DeltaTime();
		else
		{
			time = 0;
			dead = false;
			linkedPool->Revoke(this);
		}
	}
}

void AbstractEnemy::OnEnabled()
{
	if (player == nullptr) player = Game::GetInstance().FindGameObjectWithTag(ObjectTags::Player, true);
	direction = player == nullptr ? (Random::Range(0, 100) < 50 ? 1 : -1) : Mathf::Sign(player->GetTransform().Position.x - transform->Position.x);
	// DebugOut(L"Enemy: %d\n", direction);
}

void AbstractEnemy::SetPool(ObjectPool* pool)
{
	this->linkedPool = pool;
}

bool AbstractEnemy::IsDead()
{
	return dead;
}

void AbstractEnemy::UpdateDirection()
{
	if (player == nullptr) player = Game::GetInstance().FindGameObjectWithTag(ObjectTags::Player, true);
	direction = player == nullptr ? (Random::Range(0, 100) < 50 ? 1 : -1) : Mathf::Sign(player->GetTransform().Position.x - transform->Position.x);
}
