#pragma once
#include "CMario.h"
class SmallMario : public CMario
{
public:
	void Awake() override;
protected:
	void InitAnimations() override;
};

