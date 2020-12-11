#pragma once
#include "AbstractBlock.h"
#include "ItemInfo.h"

class QuestionBlock : public AbstractBlock
{
public:
	void Start() override;
	void PreRender() override;
	void Bounce(GameObject obj) override;
	void SetItem(ItemInfo item);

protected:
	virtual void InitAnimation() override;

private:
	int bouncingState;
	int lastTick;
	ItemInfo containedItem;
};

