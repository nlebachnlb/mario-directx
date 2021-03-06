#pragma once
#include "AbstractEnemy.h"
#include "PhysicConstants.h"

const Vector2 GOOMBA_BBOX(16 * 3 - 8, 16 * 3 - 10);
const float GOOMBA_MOVING_SPEED = 0.10f;
const PhysicMaterial GOOMBA_PHYSIC_MATERIAL(Vector2(GOOMBA_MOVING_SPEED, 0), 0);
const float GOOMBA_GRAVITY = DEFAULT_GRAVITY / 1.5f;
const int GOOMBA_DEAD_TIME = 800; // miliseconds
const float GOOMBA_DEFLECTION_ON_SHOT = -0.8f;

class Goomba : public AbstractEnemy
{
public:
	void Start()			override;
	virtual void Movement()			override;
	virtual void InitAnimations()	override;
	void OnDead(bool oneHit)override;
	virtual void OnEnabled() override;

	void OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider);

protected:
	Vector2 GetBoxSize()	override;
};

