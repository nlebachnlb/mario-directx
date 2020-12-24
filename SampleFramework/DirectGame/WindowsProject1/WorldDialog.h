#pragma once
#include "UIElement.h"
#include "Text.h"

class WorldDialog : public UIElement
{
public:
	void Awake() override;
	void Start() override;
	void PreRender() override;
	void Render() override;

private:
	Sprite background;
	Text* life, * world;
};

