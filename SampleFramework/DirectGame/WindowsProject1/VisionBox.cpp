#include "VisionBox.h"

void VisionBox::Awake()
{
	Collider2D* box = new Collider2D();
	box->AttachToEntity(this);
	box->SetTrigger(true);
	this->colliders->push_back(box);
	this->rigidbody->SetDynamic(true);
}
