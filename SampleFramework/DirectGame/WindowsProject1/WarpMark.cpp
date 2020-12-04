#include "WarpMark.h"

void WarpMark::Awake()
{
	SetTag(ObjectTags::WarpMark);

	Collider2D* box = new Collider2D();
	box->AttachToEntity(this);
	box->SetTrigger(true);
	this->colliders->push_back(box);
	this->rigidbody->SetDynamic(false);
}

void WarpMark::SetWarpInfo(int boundId, Vector2 dest, WarpDirection outDir, bool lockCamera)
{
	this->cameraBoundId = boundId;
	this->destination = dest;
	this->outDirection = outDir;
	this->lockCamera = lockCamera;
}

int WarpMark::GetCameraBoundId()
{
	return cameraBoundId;
}

Vector2 WarpMark::GetDestination()
{
	return this->destination;
}

WarpDirection WarpMark::GetDirection()
{
	return outDirection;
}

bool WarpMark::CameraLock()
{
	return this->lockCamera;
}
