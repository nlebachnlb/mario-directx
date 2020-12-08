#pragma once
#include "UIElement.h"
// #include "Sprite.h"
const float BLINK_TIME = 50;

class PowerMeter : public UIElement
{
public:
	void Awake() override;
	void Start() override;
	void PreRender() override;
	void Render() override;

	void SetLevel(int level);

private:
	Sprite arrowOn, powerOn;
	Sprite arrowOff, powerOff;
	int spacing;
	int blink;
	int level;
};