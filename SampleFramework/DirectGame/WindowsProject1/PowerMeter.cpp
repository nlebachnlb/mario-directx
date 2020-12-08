#include "PowerMeter.h"
#include "Game.h"
#include "Mathf.h"

void PowerMeter::Awake()
{
	auto sprMng = Game::GetInstance().GetService<SpriteManager>();
	arrowOn = sprMng->Get("spr-arrow-0");
	arrowOff = sprMng->Get("spr-arrow-1");
	powerOn = sprMng->Get("spr-p-icon-0");
	powerOff = sprMng->Get("spr-p-icon-1");
	spacing = -3;
}

void PowerMeter::Start()
{
	blink = 0;
	// DebugOut(L"PMeter: %d, %d\n", powerOn != nullptr ? 1 : 0, powerOff != nullptr ? 1 : 0);
}

void PowerMeter::PreRender()
{
	if (this->level == 7) blink += Game::DeltaTime() * Game::GetTimeScale();
	if (blink > BLINK_TIME) blink = 0;
}

void PowerMeter::Render()
{
	if (arrowOn == nullptr) return;
	if (arrowOff == nullptr) return;
	if (powerOn == nullptr) return;
	if (powerOff == nullptr) return;

	// Draw arrow
	int x = 0;
	int w = arrowOn->GetSpriteWidth();
	for (int i = 1; i <= 6; i++)
	{
		arrowOff->Draw(rectTransform.Position.x + x, rectTransform.Position.y, 0, 0);
		x += w + spacing;
	}
	powerOff->Draw(rectTransform.Position.x + x, rectTransform.Position.y, 0, 0);

	x = 0;
	w = arrowOn->GetSpriteWidth();
	for (int i = 1; i <= Mathf::Min(this->level, 6); i++)
	{
		arrowOn->Draw(rectTransform.Position.x + x, rectTransform.Position.y, 0, 0);
		x += w + spacing;
	}
	if (this->level == 7 && blink > BLINK_TIME * 0.5f) 
		powerOn->Draw(rectTransform.Position.x + x, rectTransform.Position.y, 0, 0);
}

void PowerMeter::SetLevel(int level)
{
	/*level--;
	for (int i = 0; i <= Mathf::Min(level, 6); ++i)
		active[i] = true;
	for (int i = Mathf::Min(level, 6) + 1; i <= 6; ++i)
		active[i] = false;*/
	this->level = Mathf::Clamp(level, 0, 7);
}
