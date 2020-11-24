#pragma once
#include "RedGoomba.h"

const float RED_PARAGOOMBA_HIGH_JUMP = 0.53f;
const float RED_PARAGOOMBA_LOW_JUMP = 0.2f;
const int RED_PARAGOOMBA_BREAK_TIME = 100; // miliseconds

class RedParagoomba : public RedGoomba
{
public:
	void Start() override;
	void InitAnimations() override;
	void Movement() override;
	void PreRender() override;
	void OnDead(bool oneHit) override;
	void OnCollisionEnter(Collider2D* self, std::vector<CollisionEvent*> collisions) override;

private:
	int jumpStep;
	bool onGround;
	int timer;
};

