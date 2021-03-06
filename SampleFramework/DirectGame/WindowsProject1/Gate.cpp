#include "Gate.h"
#include "Game.h"
#include "Sprite.h"
#include "Mathf.h"

void Gate::InitAnimations()
{
	auto sprMng = Game::GetInstance().GetService<SpriteManager>();

	gates[0] = sprMng->Get("spr-start-0");
	gates[1] = sprMng->Get("spr-gate-1");
	gates[2] = sprMng->Get("spr-gate-2");
	gates[3] = sprMng->Get("spr-gate-3");
	gates[4] = sprMng->Get("spr-gate-4");
	gates[5] = sprMng->Get("spr-gate-5");
	gates[6] = sprMng->Get("spr-gate-6");
	gates[7] = sprMng->Get("spr-spade-0");

	number = 0;
}

void Gate::Render(Vector2 translation)
{
	if (!Mathf::InRange(number, 0, 7)) return;
	gates[number]->Draw(transform->Position.x + translation.x, transform->Position.y + translation.y);
}

void Gate::SetNumber(int value)
{
	number = value;
}

void Gate::SetScene(std::string id)
{
	sceneID = id;
}
