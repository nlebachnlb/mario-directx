#pragma once
#include "KoopaParatroopa.h"

const float KOOPA_PARATROOPA_JUMP_VEL = 0.6f;

class GreenKoopaParatroopa : public KoopaParatroopa
{
public:
	void Start() override;
	void Movement() override;
	void InitAnimations() override;
	void OnCollisionEnter(Collider2D* self, std::vector<CollisionEvent*> collisions) override;

protected:
	bool onGround;
};

