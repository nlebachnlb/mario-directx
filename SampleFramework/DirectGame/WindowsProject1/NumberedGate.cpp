#include "NumberedGate.h"
#include "Game.h"
#include "Sprite.h"
#include "Mathf.h"

void NumberedGate::InitAnimations()
{
	auto sprMng = Game::GetInstance().GetService<SpriteManager>();

	gates[0] = sprMng->Get("spr-start-0");
	gates[1] = sprMng->Get("spr-gate-1");
	gates[2] = sprMng->Get("spr-gate-2");
	gates[3] = sprMng->Get("spr-gate-3");
	gates[4] = sprMng->Get("spr-gate-4");
	gates[5] = sprMng->Get("spr-gate-5");
	gates[6] = sprMng->Get("spr-gate-6");

	number = 0;
}

void NumberedGate::Render(Vector2 translation)
{
	if (!Mathf::InRange(number, 0, 6)) return;
	gates[number]->Draw(transform.Position.x + translation.x, transform.Position.y + translation.y);
}

void NumberedGate::SetNumber(int value)
{
	number = value;
}
