#pragma once
#include "MarioFX.h"
class TransformationFX : public MarioFX
{
public:
	virtual void Awake() override;
	void Start() override;
	void OnAnimationEnd() override;
};

