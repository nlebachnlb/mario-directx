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
	void Awake() override;
	void AddElement(DrawInfo info);
	void AddOption(Vector2 opt);
	void Render(Vector2 translation) override;
	void OnKeyDown(int keyCode) override;
private:
	std::vector<DrawInfo> elements;
	std::vector<Sprite> sprites;
	std::vector<Vector2> options;
	Sprite arrow;
	int currentOption;
};

