#pragma once
#include "UIElement.h"
// #include "Sprite.h"

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
	bool active[7];
	int spacing;
};