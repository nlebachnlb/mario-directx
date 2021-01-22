#pragma once
#include "AbstractEnemy.h"

const int BOOMERANG_BRO_MOVEDELTA = 48 * 3;
const float BOOMERANG_BRO_SPEED = 0.1f;
const Vector2	BOOMERANG_BRO_BBOX(16 * 3 - 8, 16 * 3 - 8);
const PhysicMaterial BOOMERANG_BRO_PHYSIC_MATERIAL(Vector2(BOOMERANG_BRO_SPEED, 0), 0);
const float		BOOMERANG_BRO_GRAVITY = 0.00093f * 2;
const int		BOOMERANG_BRO_DEAD_TIME = 500; // miliseconds
const int		BOOMERANG_BRO_IDLE_TIME = 500; // miliseconds
const float		BOOMERANG_BRO_DEFLECTION_ON_SHOT = -0.4f;

class BoomerangBrother : public AbstractEnemy
{
public:
	void Awake() override;
	void OnEnabled() override;
	void Start() override;
	void Movement() override;
	void OnDead(bool oneHit) override;
	void InitAnimations() override;
	void OnOverlapped(Collider2D* self, Collider2D* other) override;

	Vector2 startPosition;

protected:
	Vector2 GetBoxSize()	override;
	void UpdateDirection();
	void ThrowBoomerang();

private:
	int movingPhase, throwingPhase;
	int timer, timer1;
	bool poolRegistered;
	ObjectPool pool;
};

