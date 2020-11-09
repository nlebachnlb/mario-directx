#include "AbstractBlock.h"

void AbstractBlock::Awake()
{
	SetTag(ObjectTags::Solid);

	Collider2D* box = new Collider2D();
	box->SetBoxSize(BLOCK_BBOX);
	box->AttachToEntity(this);
	this->colliders->push_back(box);
	this->rigidbody->SetDynamic(false);

	InitAnimation();
}