#pragma once
#include "MapNode.h"
class NumberedGate : public MapNode
{
public:
	void InitAnimations() override;
	void Render(Vector2 translation) override;

	void SetNumber(int value);

private:
	int number;
	Sprite gates[7];
};

