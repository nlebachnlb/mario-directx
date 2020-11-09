#include "FXObject.h"

void FXObject::SetPool(ObjectPool* pool)
{
	this->pool = pool;
}

void FXObject::SetStartPosition(Vector2 position)
{
	this->startPosition = position;
}