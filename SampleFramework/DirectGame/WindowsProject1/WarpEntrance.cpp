#include "WarpEntrance.h"

void WarpEntrance::Awake()
{
	isStatic = true;
	SetTag(ObjectTags::WarpEntrance);

	Collider2D* box = new Collider2D();
	box->AttachToEntity(this);
	box->SetTrigger(true);
	this->colliders->push_back(box);
	this->rigidbody->SetDynamic(false);
}

void WarpEntrance::SetWarpDirection(WarpDirection direction)
{
	this->warpDirection = direction;
}

WarpDirection WarpEntrance::GetWarpDirection()
{
	return this->warpDirection;
}
