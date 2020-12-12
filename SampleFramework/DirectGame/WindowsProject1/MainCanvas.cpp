#include "MainCanvas.h"
#include "Game.h"
#include "Consts.h"
#include "Mathf.h"
#include "Brick.h"
#include "Coin.h"

void MainCanvas::Awake()
{
	hud = new HudPanel();
	hud->Initialize();
	AddUIElement(hud);

	mask = Game::GetInstance().GetService<TextureManager>()->GetTexture(TEXTURE_BOX);

	courseClear = new Text();
	reward		= new Text();

	auto font = Game::GetInstance().GetGlobalFont();
	courseClear->SetFont(font);
	reward->SetFont(font);

	courseClear->SetSpacing(0);
	reward->SetSpacing(0);

	/*AddUIElement(courseClear);
	AddUIElement(reward);*/
	auto sprManager = Game::GetInstance().GetService<SpriteManager>();
	cardVisuals[0] = sprManager->Get("spr-super-mushroom-card-0");
	cardVisuals[1] = sprManager->Get("spr-fire-flower-card-0");
	cardVisuals[2] = sprManager->Get("spr-star-man-card-0");

	gameState = GameState::Ready;
}

void MainCanvas::Start()
{
	hud->SetWorld(1);
	hud->SetScore(0);
	hud->SetCoin(0);
	hud->SetTimer(0);
	hud->SetLife(4);
	transition = 0;
	alpha = 0;

	courseClear->SetPosition(Vector2(240, 120));
	reward->SetPosition(Vector2(200, 192));

	courseClear->SetContent("");
	reward->SetContent("");
}

void MainCanvas::Update()
{
	switch (gameState)
	{
	case GameState::Run:
		GameRun();
		break;
	case GameState::Finish:
		GameFinish();
		break;
	}
}

void MainCanvas::PreRender()
{
	hud->SetLife(gameData->life);
	hud->SetWorld(gameData->world);
	hud->SetScore(gameData->score);
	hud->SetCoin(gameData->coin);

	hud->SetTimer((int)((float)time * 0.001f));
	hud->PreRender();
}

void MainCanvas::Render()
{
	Game::GetInstance().DrawTexture(0, 594, 0, 0, mask, 0, 0, 824, 150);
	Canvas::Render();
	if (gameState == GameState::Finish)
	{
		courseClear->Render();

		if (finishStep >= 1)
		{
			reward->Render();
			if (finishStep >= 2) 
				cardVisuals[card]->Draw(576, 192 - 32, 0, 0);
		}
	}

	if (alpha > 0)
	{
		auto conf = Game::GetInstance().GetGlobalConfigs();
		Game::GetInstance().DrawTexture(0, 0, 0, 0, mask, 0, 0, conf.screenWidth, conf.screenHeight, (int)alpha);
	}
}

void MainCanvas::SetGameData(GameData* data)
{
	this->gameData = data;
}

void MainCanvas::ResetTimer()
{
	time = GAME_TIME;
}

void MainCanvas::StartGame()
{
	gameState = GameState::Run;
	ResetTimer();
}

void MainCanvas::FinishGame(int card)
{
	finishTimer = 0;
	finishStep = 0;
	gameState = GameState::Finish;
	this->card = card;
}

void MainCanvas::StartTransition()
{
	transition = 1;
	DebugOut(L"Canvas Start transition: %d\n", alpha);
}

bool MainCanvas::IsSwitchTime()
{
	return pSwitchTimer > 0;
}

void MainCanvas::SwitchCoinBrick(bool freeze)
{
	auto bricks = Game::GetInstance().FindGameObjectsOfType<Brick>();
	auto coins = Game::GetInstance().FindGameObjectsOfType<Coin>();
	auto sceneManager = Game::GetInstance().GetService<SceneManager>();
	auto scene = sceneManager->GetActiveScene();

	for (auto brick : bricks)
	{
		// DebugOut(L"brick\n");
		auto coin = Instantiate<Coin>();
		coin->SetPosition(brick->GetTransform().Position);
		coin->SetFreeze(freeze);
		scene->AddObject(coin);
		Destroy(brick);
	}

	for (auto coin : coins)
	{
		// DebugOut(L"brick\n");
		auto brick = Instantiate<Brick>();
		brick->SetPosition(coin->GetTransform().Position);
		brick->SetFreeze(freeze);
		scene->AddObject(brick);
		Destroy(coin);
	}
}

void MainCanvas::StartSwitchTimer()
{
	pSwitchTimer = PSWITCH_TIME;
	timeFreeze = true;
}

void MainCanvas::GameRun()
{
	if (player == nullptr)
	{
		auto obj = Game::GetInstance().FindGameObjectWithTag(ObjectTags::Player);
		if (obj != nullptr) player = static_cast<PlayerController*>(obj);
		else return;
	}

	auto mario = player->GetMario();
	auto percent = mario->GetPMeter();
	int powerLevel = (int)(percent);
	hud->SetPowerMeter(powerLevel);
	auto dt = Game::DeltaTime();

	auto dtScaled = Game::DeltaTime() * Game::GetTimeScale();
	time -= dtScaled * 1.5f;

	if (timeFreeze)
	{
		pSwitchTimer -= dtScaled;
		if (pSwitchTimer <= 0)
		{
			pSwitchTimer = 0;
			timeFreeze = false;
			SwitchCoinBrick(false);
		}
	}

	switch (transition)
	{
	case 1:
	{
		alpha += (255.0f / (float)TRANSITION_TIME) * dt;
		if (alpha >= 255)
		{
			alpha = 255;
			transition = 2;
			mario->Warp();
		}
	}
	break;
	case 2:
	{
		alpha -= (255.0f / (float)TRANSITION_TIME) * dt;
		if (alpha <= 1)
		{
			alpha = 0;
			transition = 3;
			mario->WarpOut();
		}
	}
	break;
	case 3:
	{
		alpha = 0;
		transition = 0;
	}
	break;
	}
}

void MainCanvas::GameFinish()
{
	hud->SetPowerMeter(0);
	// DebugOut(L"Game state\n");

	auto dt = Game::DeltaTime() * Game::GetTimeScale();

	switch (finishStep)
	{
	case 0:
	{
		finishTimer += dt;
		if (finishTimer > 2000)
		{
			// Show COURSE CLEAR text
			DebugOut(L"COURSE CLEAR\n");
			finishTimer = 0;
			courseClear->SetContent(COURSE_CLEAR);
			finishStep = 1;
		}
	}
	break;
	case 1:
	{
		finishTimer += dt;
		if (finishTimer > 900)
		{
			// Show reward: YOU GOT A CARD
			DebugOut(L"YOU GOT A CARD\n");
			finishTimer = 0;
			reward->SetContent(CARD_REWARD);
			// Show card

			// Blink card in HUD
			Game::GetInstance().GetData()->GetCards()->push_back(card + 1);
			hud->StartBlinkingLastCard();
			finishStep = 2;
		}
	}
	break;
	case 2:
	{
		finishTimer += dt;
		if (finishTimer > 1000)
			finishTimer = 0, finishStep = 3;
	}
	break;
	case 3:
	{
		finishTimer += dt;

		if (finishTimer > 30)
		{
			auto data = Game::GetInstance().GetData();
			finishTimer = 0;
			if (time > 99 * 1000) time -= 10 * 1000, data->ModifyScore(500, true);
			else if (time > 0) time -= 1000, data->ModifyScore(50, true);
			else
			{
				finishTimer = 0;
				finishStep = 4;
			}
		}
	}
	break;
	}
}
