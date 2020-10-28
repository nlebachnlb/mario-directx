#pragma once
#include "AbstractEnemy.h"

const Vector2 GOOMBA_BBOX(16 * 3 - 8, 16 * 3 - 10);
const float GOOMBA_MOVING_SPEED = 0.05f;
const PhysicMaterial GOOMBA_PHYSIC_MATERIAL(Vector2(GOOMBA_MOVING_SPEED, 0), 0);
const float GOOMBA_GRAVITY = 0.00093f;

class Goomba : public AbstractEnemy
{
public:
	void Start()			override;
	void Movement()			override;
	void InitAnimations()	override;
	void OnDead()			override;

	void OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions) override;

protected:
	Vector2 GetBoxSize()	override;
};

