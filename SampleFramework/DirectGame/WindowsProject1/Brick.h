#pragma once
#include "AbstractBlock.h"
class Brick : public AbstractBlock
{
public:
	void Start() override;
	void InitAnimation() override;
	void PreRender() override;
	void Bounce(GameObject obj) override;
	void OnOverlapped(Collider2D*, Collider2D*);

private:
	void Explode();
	int bouncingState;
	DWORD lastTick;
};

