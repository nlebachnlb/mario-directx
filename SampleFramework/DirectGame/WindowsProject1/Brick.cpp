#include "Brick.h"
#include "AnimationDatabase.h"
#include "Game.h"

void Brick::Start()
{
	SetState("Default");
	bouncingState = 0;
}

void Brick::InitAnimation()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-brick"));
}

void Brick::PreRender()
{
	switch (bouncingState)
	{
	case 1:
	{
		visualRelativePosition.y -= BOUNCE_VEL * Game::DeltaTime();
		if (GetTickCount() - lastTick > BOUNCE_TIME)
			lastTick = GetTickCount(), bouncingState = 2;
	}
	break;
	case 2:
	{
		visualRelativePosition.y += BOUNCE_VEL * Game::DeltaTime();
		if (GetTickCount() - lastTick > BOUNCE_TIME)
			visualRelativePosition.y = 0, bouncingState = 0;
	}
	break;
	}
}

void Brick::Bounce(GameObject obj)
{
	auto oTag = obj->GetTag();

	if (oTag == ObjectTags::SmallMario && bouncingState == 0)
		lastTick = GetTickCount(), bouncingState = 1;
	else
		Explode();
}

void Brick::Explode()
{
	Destroy(this);
}
