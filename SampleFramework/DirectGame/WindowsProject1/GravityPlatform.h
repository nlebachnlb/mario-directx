#pragma once
#include "AbstractPlatform.h"

class GravityPlatform : public AbstractPlatform
{
public:
	void InitAnimations() override;
	Vector2 GetBoxSize() override;
	void Update() override;
};

