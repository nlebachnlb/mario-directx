#pragma once
#include "AbstractBlock.h"
#include "ItemInfo.h"
#include "PlayerController.h"

class QuestionBlock : public AbstractBlock
{
public:
	void OnEnabled() override;
	void Start() override;
	void LateUpdate() override;
	void Bounce(GameObject obj) override;
	void SetItem(ItemInfo item);

protected:
	virtual void InitAnimation() override;

private:
	int bouncingState;
	int lastTick;
	ItemInfo containedItem;
	PlayerController* player;

};

