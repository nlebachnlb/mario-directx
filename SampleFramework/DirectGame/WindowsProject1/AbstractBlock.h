#pragma once
#include "GameObject.h"

const Vector2 BLOCK_BBOX(48, 48);

class AbstractBlock : public CGameObject
{
public:
	void Awake() override;

protected:
	virtual void InitAnimation() = 0;
};

