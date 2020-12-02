#pragma once
#include "Canvas.h"
#include <unordered_map>
#include "Animation.h"
class Canvas;
class UIElement
{
public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void PreRender();
	virtual void Render();

	void SetCanvas(Canvas* canvas);
	void SetState(std::string state);
	Animation* GetState(std::string state);
	void SetActive(bool active);
	bool IsActive();

protected:
	bool enabled;
	Canvas* canvasSubject;
	Transform rectTransform;
	int renderOrder;

	std::string currentState, lastState;
	std::unordered_map<std::string, Animation*> animations;
	Vector2 visualRelativePosition;
};

