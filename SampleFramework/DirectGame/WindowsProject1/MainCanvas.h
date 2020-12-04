#pragma once
#include "Canvas.h"
#include "HudPanel.h"
class MainCanvas : public Canvas
{
public:
	void Awake() override;
	void Start() override;
private:
	HudPanel* hud;
};

