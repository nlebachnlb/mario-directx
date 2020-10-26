#pragma once
#include "GameObject.h"
#include "PhysicMaterial.h"
#include "ObjectPool.h"

const Vector2 FIREBALL_BBOX(8, 8);
const float FIREBALL_DEFLECTION = 0.25f;
const float FIREBALL_GRAVITY = 0.00093f;
const PhysicMaterial FIREBALL_MATERIAL(FIREBALL_DEFLECTION, 0);

class MarioFireball : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions) override;

	void LinkToPool(ObjectPool* pool);
private:
	ObjectPool* pool;
};

