#include "AbstractBlock.h"

void AbstractBlock::Awake()
{
	beingBumped = false;
	isStatic = true;
	SetTag(ObjectTags::Block);

	Collider2D* box = new Collider2D();
	box->SetBoxSize(BLOCK_BBOX);
	box->AttachToEntity(this);
	this->colliders->push_back(box);
	this->rigidbody->SetDynamic(false);

	InitAnimation();
}

void AbstractBlock::SetFreeze(bool value)
{
	timeFreeze = value;
}

bool AbstractBlock::IsBumped()
{
	return beingBumped;
}
