#include "SceneAnimator.h"
#include "Game.h"
#include "IntroTitle.h"
#include "MenuVersionFX.h"

void SceneAnimator::Awake()
{
	cutOrder = 0;
	alwaysUpdate = true;
	auto spr = Game::GetInstance().GetService<SpriteManager>();
	sprites[0] = spr->Get("spr-intro-ground-0");
	sprites[1] = spr->Get("spr-intro-ground-1");
	sprites[2] = spr->Get("spr-intro-ground-2");
	sprites[3] = spr->Get("spr-full-curtain-0");
}

void SceneAnimator::Start()
{
	elapsedTime = 0;
	curtainPos = Vector2(0, CURTAIN_START - 584);
}

void SceneAnimator::Update()
{
	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	switch (cutOrder)
	{
	case 0:
	{
		elapsedTime += dt;
		if (elapsedTime > 500)
		{
			elapsedTime = 0;
			cutOrder = 1;
		}
	}
	break;
	case 1:
	{
		float speed = CURTAIN_START / 1500.0f;
		curtainPos.y -= speed * dt;
		if (curtainPos.y < -CURTAIN_START)
		{
			curtainPos.y = -CURTAIN_START;
			cutOrder = 2;
			elapsedTime = 0;

			// Create title
			auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
			auto title = Instantiate<IntroTitle>();
			auto version = Instantiate<MenuVersionFX>();
			title->SetVersion(version);
			scene->AddObject(title);
			scene->AddObject(version);
		}
	}
	break;
	case 2:
	{
		elapsedTime += dt;
		if (elapsedTime > MOVEMENT_DURATION)
		{
			elapsedTime = 0;
			cutOrder = 3;
		}
	}
	break;
	}
}

void SceneAnimator::Render(Vector2 translation)
{
	// Draw floor
	sprites[0]->Draw(000, 642, 0, 0);
	sprites[1]->Draw(768, 642, 0, 0);
	sprites[2]->Draw(816, 642, 0, 0);

	// Curtain
	sprites[3]->Draw(curtainPos.x, curtainPos.y - (CURTAIN_START - 584), 0, 0);
	sprites[3]->Draw(curtainPos.x, curtainPos.y, 0, 0);
}
