#include "QuestionBlock.h"
#include "AnimationDatabase.h"
#include "Game.h"

void QuestionBlock::Start()
{
	SetState("Sealed");
}

void QuestionBlock::InitAnimation()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Sealed", animations->Get("ani-question-block"));
	AddAnimation("Empty", animations->Get("ani-empty-block"));
}
