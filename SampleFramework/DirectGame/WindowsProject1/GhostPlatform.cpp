#include "GhostPlatform.h"

void GhostPlatform::Awake()
{
	global = true;
	SetTag(ObjectTags::GhostPlatform);
	SetEffector(Effector2D::Top);

	Collider2D* box = new Collider2D();
	box->AttachToEntity(this);
	this->colliders->push_back(box);
	this->rigidbody->SetDynamic(false);
}
