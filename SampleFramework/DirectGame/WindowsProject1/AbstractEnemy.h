#pragma once
#include "GameObject.h"
#include <unordered_map>
#include "ObjectPool.h"

class AbstractEnemy : public CGameObject
{
public:
	virtual void Awake() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void OnEnabled() override;
	virtual void OnCollisionEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions);

	virtual void Movement() = 0;
	virtual void InitAnimations() = 0;
	virtual void OnDead(bool oneHit) = 0;

	void SetPool(ObjectPool* pool);
	bool IsDead();

protected:
	virtual void UpdateDirection();
	ObjectPool* linkedPool;
	virtual Vector2 GetBoxSize() = 0;
	GameObject player;
	float time;
	bool dead;
	int facing, direction;
	bool bumpOneHit = true;
};

