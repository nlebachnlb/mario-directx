#pragma once
#include "AbstractEnemy.h"

const Vector2	KOOPA_PARATROOPA_BBOX(16 * 3 - 8, 27 * 3 - 8);
const float		KOOPA_PARATROOPA_MOVING_SPEED = 0.09f;
const PhysicMaterial KOOPA_PARATROOPA_PHYSIC_MATERIAL(Vector2(KOOPA_PARATROOPA_MOVING_SPEED, 0), 0);
const float		KOOPA_PARATROOPA_GRAVITY = 0.002f;
const int		KOOPA_PARATROOPA_DEAD_TIME = 800; // miliseconds
const float		KOOPA_PARATROOPA_DEFLECTION_ON_SHOT = -0.4f;

enum class Troopa
{
	Red, Green
};

class KoopaParatroopa : public AbstractEnemy
{
public:
	virtual void OnDead(bool oneHit) override;
	virtual void InitAnimations() = 0;
	virtual void Movement() = 0;
	virtual void OnEnabled() override;

protected:
	Vector2 GetBoxSize() override;
	Troopa troopa;
};

