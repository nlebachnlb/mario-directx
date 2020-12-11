#pragma once
#include "GameObject.h"

const Vector2 BLOCK_BBOX(48, 48);
const int BOUNCE_TIME = 50;
const int BOUNCE_DELTA = 24; // pixels
const float BOUNCE_VEL = (float)BOUNCE_DELTA / (float)BOUNCE_TIME;

class AbstractBlock : public CGameObject
{
public:
	void Awake() override;
	virtual void Bounce(GameObject obj) = 0;
	void SetFreeze(bool value);

protected:
	virtual void InitAnimation() = 0;
	bool timeFreeze;
};

