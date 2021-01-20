#include "SceneAnimator.h"
#include "Game.h"
#include "IntroTitle.h"
#include "MenuVersionFX.h"
#include "PlayerController.h"
#include "IntroMarioController.h"

void SceneAnimator::Awake()
{
	cutOrder = 0;
	alwaysUpdate = true;
	auto spr = Game::GetInstance().GetService<SpriteManager>();
	sprites[0] = spr->Get("spr-intro-ground-0");
	sprites[1] = spr->Get("spr-intro-ground-1");
	sprites[2] = spr->Get("spr-intro-ground-2");
	sprites[3] = spr->Get("spr-full-curtain-0");
	sprites[4] = spr->Get("spr-bottom-curtain-0");
	sprites[5] = spr->Get("spr-tree-left");
	sprites[6] = spr->Get("spr-tree-right");

	mask = Game::GetInstance().GetService<TextureManager>()->GetTexture(TEXTURE_BOX);
}

void SceneAnimator::Start()
{
	elapsedTime = 0;
	curtainPos = Vector2(0, CURTAIN_START - 584);
	maskAlpha = 255;
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
			
			auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
			auto botPlayer = Instantiate<IntroMarioController>();
			scene->AddObject(botPlayer);
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
		if (elapsedTime > MOVEMENT_DURATION + 700)
		{
			elapsedTime = 0;
			cutOrder = 3;
		}
	}
	break;
	case 3:
	{
		maskAlpha -= (255.0f / 400.0f) * dt;
		if (maskAlpha < 0)
		{
			maskAlpha = 0;
			cutOrder = 4;
		}
	}
	break;
	}
}

void SceneAnimator::Render(Vector2 translation)
{
	sprites[4]->Draw(0, 0, 0, 0);
	sprites[5]->Draw(0, 642, 0, 192);
	sprites[6]->Draw(635, 642, 0, 288);

	// Draw black mask
	if (maskAlpha > 0)
		Game::GetInstance().DrawTexture(0, 0, 0, 0, mask, 0, 0, 824, 744, maskAlpha);

	// Draw floor
	sprites[0]->Draw(000, 642, 0, 0);
	sprites[1]->Draw(768, 642, 0, 0);
	sprites[2]->Draw(816, 642, 0, 0);

	// Curtain
	sprites[3]->Draw(curtainPos.x, curtainPos.y - (CURTAIN_START - 584), 0, 0);
	sprites[3]->Draw(curtainPos.x, curtainPos.y, 0, 0);
}
