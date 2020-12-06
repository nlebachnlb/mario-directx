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
	for (int i = 0; i < 7; ++i) active[i] = false;
}

void PowerMeter::PreRender()
{
	
}

void PowerMeter::Render()
{
	if (arrowOn == nullptr) return;
	if (arrowOff == nullptr) return;

	// Draw arrow
	int x = 0;
	for (int i = 0; i < 5; i++)
	{
		auto target = active[i] ? arrowOn : arrowOff;
		target->Draw(rectTransform.Position.x + x, rectTransform.Position.y, 0, 0);
		x += target->GetSpriteWidth() + spacing;
	}
	auto target = active[6] ? powerOn : powerOff;
	target->Draw(rectTransform.Position.x + x, rectTransform.Position.y, 0, 0);
}

void PowerMeter::SetLevel(int level)
{
	level--;
	for (int i = 0; i <= Mathf::Min(level, 6); ++i)
		active[i] = true;
	for (int i = Mathf::Min(level, 6) + 1; i <= 6; ++i)
		active[i] = false;
}
