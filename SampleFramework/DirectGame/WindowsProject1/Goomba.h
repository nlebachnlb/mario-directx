#pragma once
#include "AbstractEnemy.h"
class Goomba : public AbstractEnemy
{
public:
	void Movement() override;
	void InitAnimations() override;
	void OnDead() override;
};

