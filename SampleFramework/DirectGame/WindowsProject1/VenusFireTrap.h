#pragma once
#include "Plant.h"

const int VENUS_WAIT_TIME = 1700;
const Vector2 VENUS_BBOX(16 * 3, 32 * 3);
const float VENUS_SPEED = 0.13f;
const float VENUS_HIDE_DISTANCE = 48 * 2;
const int VENUS_N_POOLED_BULLETS = 2;
const float VENUS_BULLET_SPEED = 0.18f;
const int VENUS_SHOOT_WAIT_TIME = VENUS_WAIT_TIME / 2;

class VenusFireTrap : public Plant
{
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void PreRender() override;
	virtual void LateUpdate() override;
	virtual void OnRevealed() override;
	virtual void OnEnabled() override;
	virtual void InitAnimations() = 0;

	ObjectPool& GetBullets();

protected:
	Vector2 GetBoxSize() override;
	void UpdateDirection() override;

	int verticalDirection;
	bool poolRegistered;
	ObjectPool bulletPool;
	int shootTimer;
	bool targetLocking;
};

