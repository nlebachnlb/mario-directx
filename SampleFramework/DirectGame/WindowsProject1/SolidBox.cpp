#include "SolidBox.h"

void SolidBox::Awake()
{
	global = true;
	SetTag(ObjectTags::Solid);

	Collider2D* box = new Collider2D();
	box->AttachToEntity(this);
	this->colliders->push_back(box);
	this->rigidbody->SetDynamic(false);
}
