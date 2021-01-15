#pragma once
#include "AbstractPlatform.h"

class GravityPlatform : public AbstractPlatform
{
public:
	void Start() override;
	void InitAnimations() override;
	Vector2 GetBoxSize() override;
	void Update() override;
	void OnTouch() override;

private:
	bool touched;
};

