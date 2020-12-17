#pragma once
#include "GameObject.h"
#include <string>
#include <vector>

struct DrawInfo
{
	std::string spriteID;
	Vector2 position, pivot;
};

class MenuBackground : public CGameObject
{
public:
	void AddElement(DrawInfo info);
	void Render(Vector2 translation) override;
private:
	std::vector<DrawInfo> elements;
	std::vector<Sprite> sprites;
};

