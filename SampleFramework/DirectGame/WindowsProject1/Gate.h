#pragma once
#include "MapNode.h"
class Gate : public MapNode
{
public:
	void InitAnimations() override;
	void Render(Vector2 translation) override;

	void SetNumber(int value);
	void SetScene(std::string id);

private:
	int number;
	Sprite gates[7];
	std::string sceneID;
};

