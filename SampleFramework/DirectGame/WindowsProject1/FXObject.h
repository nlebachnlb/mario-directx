#pragma once
#include "ObjectPool.h"

class FXObject : public CGameObject
{
public:
	void SetPool(ObjectPool* pool);
	void SetStartPosition(Vector2 position);

protected:
	ObjectPool* pool;
	Vector2 startPosition;
};

