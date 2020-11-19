#pragma once
#include "AbstractItem.h"
#include "Camera.h"

const int LEAF_AMPLITUDE = 48; // px
const int LEAF_ROUNDTRIP_TIME = 500;
const float LEAF_REVEAL_DISTANCE = 48 * 2.5f; // px
const float LEAF_REVEAL_FORCE = 0.4f;
const float LEAF_GRAVITY = 0.0012f;
const float LEAF_FALLING_VEL = 0.065f;

class RaccoonLeaf : public AbstractItem
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void PreRender() override;
	void SproutOut() override;

private:
	Vector2 startPosition, prevVisualPos;
	bool falling;
	int lifeTime;
};

