#include "AbstractEnemy.h"

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
