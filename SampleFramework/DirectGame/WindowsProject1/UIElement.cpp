#include "UIElement.h"

UIElement::UIElement()
{
	this->enabled = false;
}

void UIElement::Awake()
{
}

void UIElement::Start()
{
}

void UIElement::Update()
{
}

void UIElement::PreRender()
{
}

void UIElement::Render()
{
	bool stateExistence = animations.find(currentState) != animations.end();
	if (animations.empty() || stateExistence == false) return;

	if (animations.find(lastState) != animations.end())
		animations.at(lastState)->SetRelativePosition(rectTransform.Position + visualRelativePosition);
	animations.at(currentState)->SetRelativePosition(rectTransform.Position + visualRelativePosition);
	animations.at(currentState)->SetScale(rectTransform.Scale);
	animations.at(currentState)->SetRotation(rectTransform.Rotation);
}

void UIElement::Initialize()
{
	enabled = false;
	Awake();
	Start();
	enabled = true;
}

void UIElement::SetCanvas(Canvas* canvas)
{
	this->canvasSubject = canvas;
}

void UIElement::SetState(std::string state)
{
	if (animations.find(state) != animations.end())
	{
		lastState = currentState;
		currentState = state;
		animations.at(state)->Play(false);
		return;
	}
	DebugOut(L"Couldnt find animation[]\n");
}

Animation* UIElement::GetState(std::string state)
{
	if (animations.find(state) != animations.end())
		return animations.at(state);
	return nullptr;
}

void UIElement::SetActive(bool active)
{
	this->enabled = active;
}

bool UIElement::IsActive()
{
	return this->enabled;
}

int UIElement::GetRenderOrder()
{
	return this->renderOrder;
}
