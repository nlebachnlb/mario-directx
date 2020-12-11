#pragma once
#include "GameObject.h"
#include "PhysicMaterial.h"
#include "ObjectPool.h"
#include "Camera.h"

const Vector2 FIREBALL_BBOX(8, 8);
const float FIREBALL_DEFLECTION = 0.45f;
const float FIREBALL_GRAVITY = 0.00098f * 2;
const PhysicMaterial FIREBALL_MATERIAL(Vector2(0, FIREBALL_DEFLECTION), 0);

class MarioFireball : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions) override;
	void OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider) override;
	void LateUpdate() override;
	void OnOffScreen() override;

	void LinkToPool(ObjectPool* pool);
private:
	void Explode();
	ObjectPool* pool; 
	Camera* mainCamera;
}; 

