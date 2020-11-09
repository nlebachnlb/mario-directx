#pragma once
#include "AbstractBlock.h"

const int BOUNCE_TIME = 100;
const int BOUNCE_DELTA = 16; // pixels
const float BOUNCE_VEL = (float)BOUNCE_DELTA / (float)BOUNCE_TIME;

class QuestionBlock : public AbstractBlock
{
public:
	void Start() override;
	void LateUpdate() override;
	void Bounce();

protected:
	void InitAnimation() override;

private:
	int bouncingState;
	int lastTick;
};

