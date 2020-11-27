#pragma once
#include "Plant.h"

const int VENUS_WAIT_TIME = 1700;
const Vector2 VENUS_BBOX(16 * 3, 32 * 3);
const float VENUS_SPEED = 0.13f;
const float VENUS_HIDE_DISTANCE = 48 * 2;

class VenusFireTrap : public Plant
{
public:
	virtual void Start() override;
	virtual void PreRender() override;
	virtual void LateUpdate() override;
	virtual void OnRevealed() override;
	virtual void InitAnimations() = 0;

protected:
	Vector2 GetBoxSize() override;
	void UpdateDirection() override;

	int verticalDirection;
};

