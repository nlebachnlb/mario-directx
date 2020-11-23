#pragma once
#include "FXObject.h"
class HitFX : public FXObject
{
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void OnAnimationEnd() override;
	virtual void InitAnimations() = 0;
};

