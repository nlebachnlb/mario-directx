#pragma once
#include "AbstractEnemy.h"

const Vector2	KOOPA_BBOX(16 * 3 - 8, 16 * 3 - 10);
const float		KOOPA_MOVING_SPEED			= 0.05f;
const PhysicMaterial KOOPA_PHYSIC_MATERIAL(Vector2(KOOPA_MOVING_SPEED, 0), 0);
const float		KOOPA_GRAVITY				= 0.00093f;
const int		KOOPA_DEAD_TIME				= 500; // miliseconds
const float		KOOPA_DEFLECTION_ON_SHOT	= -0.4f;

class RedKoopa : public AbstractEnemy
{
public:
	void Start()			override;
	void Movement()			override;
	void InitAnimations()	override;
	void OnDead(bool oneHit)override;

	void OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider);

protected:
	Vector2 GetBoxSize()	override;
};

