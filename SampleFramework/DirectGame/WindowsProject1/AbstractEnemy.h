#pragma once
#include "GameObject.h"
#include <unordered_map>
#include "ObjectPool.h"

class AbstractEnemy : public CGameObject
{
public:
	virtual void Awake() override;
	virtual void Update() override;

	virtual void Movement() = 0;
	virtual void InitAnimations() = 0;
	virtual void OnDead(bool oneHit) = 0;

	void SetPool(ObjectPool* pool);

protected:
	ObjectPool* linkedPool;
	virtual Vector2 GetBoxSize() = 0;
};

