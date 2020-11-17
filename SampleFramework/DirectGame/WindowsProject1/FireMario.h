#pragma once
#include "CMario.h"
#include "ObjectPool.h"

const float FIREBALL_VELOCITY = 0.45f;

class FireMario : public CMario
{
public:
	void Awake() override;
	void Start() override;
	void OnKeyDown(int keyCode) override;
	void OnAnimationEnd() override;

	ObjectPool GetFireballs();
protected:
	void InitAnimations() override;
	void MovementAnimation() override;
private:
	bool attacking;
	ObjectPool fireballs;
};

