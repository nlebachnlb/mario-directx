#pragma once
#include "AbstractEnemy.h"

const Vector2	KOOPA_BBOX(16 * 3 - 8, 27 * 3 - 8);
const float		KOOPA_MOVING_SPEED			= 0.05f * 2;
const PhysicMaterial KOOPA_PHYSIC_MATERIAL(Vector2(KOOPA_MOVING_SPEED, 0), 0);
const float		KOOPA_GRAVITY				= 0.00093f * 2;
const int		KOOPA_DEAD_TIME				= 500; // miliseconds
const float		KOOPA_DEFLECTION_ON_SHOT	= -0.4f;

class RedKoopa : public AbstractEnemy
{
public:
	void Start()			override;
	void Movement()			override;
	virtual void InitAnimations()	override;
	virtual void OnDead(bool oneHit)override;
	virtual void OnEnabled() override;

	virtual void OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions);
	void OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider);

protected:
	Vector2 GetBoxSize()	override;
};

