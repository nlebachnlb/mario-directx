#pragma once
#include "AbstractBlock.h"
#include "ItemInfo.h"

const int BOUNCE_TIME = 80;
const int BOUNCE_DELTA = 32; // pixels
const float BOUNCE_VEL = (float)BOUNCE_DELTA / (float)BOUNCE_TIME;

class QuestionBlock : public AbstractBlock
{
public:
	void Start() override;
	void PreRender() override;
	void Bounce();
	void SetItem(ItemInfo item);

protected:
	void InitAnimation() override;

private:
	int bouncingState;
	int lastTick;
	ItemInfo containedItem;
};

