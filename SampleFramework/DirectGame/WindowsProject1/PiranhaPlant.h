#pragma once
#include "Plant.h"

const int PIRANHA_WAIT_TIME = 800;
const Vector2 PIRANHA_BBOX(16 * 3, 24 * 3);
const float PIRANHA_SPEED = 0.15f;
const float PIRANHA_HIDE_DISTANCE = 48 * 2;

class PiranhaPlant : public Plant
{
public:
	void InitAnimations() override;
	void Start() override;

protected:
	Vector2 GetBoxSize();
};

