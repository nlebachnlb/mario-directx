#include "AbstractEnemy.h"
#include "Game.h"

void AbstractEnemy::Awake()
{
	SetTag(ObjectTags::Enemy);

	InitAnimations();

	Collider2D* box = new Collider2D();
	box->SetBoxSize(GetBoxSize());
	box->AttachToEntity(this);
	this->colliders->push_back(box);

	rigidbody->SetDynamic(true);
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

void AbstractEnemy::SetPool(ObjectPool* pool)
{
	this->linkedPool = pool;
}
