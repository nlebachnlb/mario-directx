#pragma once
#include "Canvas.h"
#include "HudPanel.h"
#include "Text.h"

class MainCanvas : public Canvas
{
public:
	void Awake() override;
	void Start() override;
	void Render() override;
private:
	HudPanel* hud;
	Texture2D mask;
};

