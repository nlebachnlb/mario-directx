#pragma once
#include "UIElement.h"
class GameOverDialog : public UIElement
{
public:
	void Awake() override;
	void Start() override;
	void PreRender() override;
	void Render() override;
	void OnKeyDown(int keyCode) override;

private:
	Sprite background, arrow;
	std::vector<Vector2> options;
	int option;
};

