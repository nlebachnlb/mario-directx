#pragma once
#include "MarioFX.h"
class TransformationFX : public MarioFX
{
public:
	void Awake() override;
	void Start() override;
	void OnAnimationEnd() override;
};

