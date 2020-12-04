#pragma once
#include "UIElement.h"
#include "Sprite.h"

class HudPanel : public UIElement
{
public:
	void Awake() override;
	void Start() override;
	void Render() override;

private:
	Sprite panel;
};