#pragma once
#include "Transform.h"
#include "UIElement.h"
#include <vector>

class UIElement;
class Canvas
{
public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void PreRender();
	virtual void Render();

	void AddUIElement(UIElement* uiObj);

protected:
	std::vector<UIElement*> elements;
};

