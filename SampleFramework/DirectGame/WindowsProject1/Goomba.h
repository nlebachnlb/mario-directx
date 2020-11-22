#pragma once
#include "AbstractEnemy.h"

const Vector2 GOOMBA_BBOX(16 * 3 - 8, 16 * 3 - 10);
const float GOOMBA_MOVING_SPEED = 0.1f;
const PhysicMaterial GOOMBA_PHYSIC_MATERIAL(Vector2(GOOMBA_MOVING_SPEED, 0), 0);
const float GOOMBA_GRAVITY = 0.0024;
const int GOOMBA_DEAD_TIME = 800; // miliseconds
const float GOOMBA_DEFLECTION_ON_SHOT = -0.4f;

class Goomba : public AbstractEnemy
{
public:
	void Start()			override;
	void Movement()			override;
	virtual void InitAnimations()	override;
	void OnDead(bool oneHit)override;
	virtual void OnEnabled() override;

	void OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions) override;
	void OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider);

protected:
	Vector2 GetBoxSize()	override;
};

