#include "ScoreFX.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "Consts.h"

void ScoreFX::Awake()
{
	level = 0;
	auto sprMng = Game::GetInstance().GetService<SpriteManager>();
	auto prefix = "spr-points-in-level-";
	std::string suffix[9] = { "100", "200", "400", "800", "1000", "2000", "4000", "8000", "1UP" };
	for (int i = 0; i < 9; ++i)
		scoreLevel[i] = sprMng->Get(prefix + suffix[i]);

	rigidbody->SetDynamic(false);
	renderOrder = 20;
}

void ScoreFX::Start()
{
	this->speed = FLOATING_SPEED;
}

void ScoreFX::PreRender()
{
	speed -= DRAG * Game::DeltaTime();
	if (speed <= 0)
	{
		pool->Revoke(this);
	}
	transform->Position.y -= speed * Game::DeltaTime();
}

void ScoreFX::Render(Vector2 translation)
{
	if (level >= 9) return;
	scoreLevel[level]->Draw(transform->Position.x + translation.x, transform->Position.y + translation.y);
}

void ScoreFX::SetLevel(Score level)
{
	this->level = (int)level;
	if (this->level < 9)
		Game::GetInstance().GetData()->ModifyScore(SCORE_VALUES[this->level], true);
}
