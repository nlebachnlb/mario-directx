#include "MainCanvas.h"
#include "Game.h"
#include "Consts.h"
#include "Mathf.h"
#include "Brick.h"
#include "Coin.h"
#include "ExampleScene.h"
#include "WorldMapScene.h"
#include "MainMenuScene.h"

void MainCanvas::Awake()
{
	wipePosition = VectorZero();

	hud = new HudPanel();
	hud->Initialize();
	AddUIElement(hud);

	worldDialog = new WorldDialog();
	worldDialog->Initialize();
	AddUIElement(worldDialog);

	gameOverDialog = new GameOverDialog();
	gameOverDialog->Initialize();
	AddUIElement(gameOverDialog);

	mask = Game::GetInstance().GetService<TextureManager>()->GetTexture(TEXTURE_BOX);

	courseClear = new Text();
	reward = new Text();
	pause = new Text();

	auto font = Game::GetInstance().GetGlobalFont();
	courseClear->SetFont(font);
	reward->SetFont(font);
	pause->SetFont(font);

	courseClear->SetSpacing(0);
	reward->SetSpacing(0);
	pause->SetSpacing(0);

	/*AddUIElement(courseClear);
	AddUIElement(reward);*/
	auto sprManager = Game::GetInstance().GetService<SpriteManager>();
	cardVisuals[0] = sprManager->Get("spr-super-mushroom-card-0");
	cardVisuals[1] = sprManager->Get("spr-fire-flower-card-0");
	cardVisuals[2] = sprManager->Get("spr-star-man-card-0");

	gameState = GameState::Menu;
}

void MainCanvas::Start()
{
	hud->SetWorld(1);
	hud->SetScore(0);
	hud->SetCoin(0);
	hud->SetTimer(300);
	hud->SetLife(4);
	transition = 0;
	alpha = 0;
	time = GAME_TIME;

	courseClear->SetPosition(Vector2(240, 120));
	reward->SetPosition(Vector2(200, 192));
	pause->SetPosition(Vector2(412 - 48, 384));

	courseClear->SetContent("");
	reward->SetContent("");
	pause->SetContent("");

	auto config = Game::GetInstance().GetGlobalConfigs();
	worldDialog->SetPosition(Vector2(config.screenWidth * 0.5f - 192, config.screenHeight * 0.5f - 96 - 96));
	worldDialog->SetActive(false);

	gameOverDialog->SetPosition(Vector2(config.screenWidth * 0.5f - 192, config.screenHeight * 0.5f - 96 - 96));
	gameOverDialog->SetActive(false);

	gameState = GameState::Menu;
}

void MainCanvas::Update()
{
	// DebugOut(L"GAME STATE %d\n", gameState == GameState::Unload ? 1 : 0);
	if (Game::GetInstance().IsPaused() && IsIngame())
	{
		if (pause->GetContent().empty()) pause->SetContent("PAUSED");
	}
	else if (!pause->GetContent().empty()) pause->SetContent("");

	switch (gameState)
	{
	case GameState::Run:
		GameRun();
		break;
	case GameState::Finish:
		GameFinish();
		break;
	case GameState::Ready:
		GameReady();
		break;
	case GameState::Die:
		GameLose();
		break;
	case GameState::Menu:
		GameMenu();
		break;
	case GameState::WorldStart:
		GameWorldStart();
		break;
	case GameState::GameOver:
		GameOver();
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

	worldDialog->PreRender();
}

void MainCanvas::Render()
{
	if (gameState != GameState::Menu)
		Game::GetInstance().DrawTexture(0, 594, 0, 0, mask, 0, 0, 824, 150);

	Canvas::Render();

	switch (gameState)
	{
	case GameState::Finish:
	{
		courseClear->Render();

		if (finishStep >= 1)
		{
			reward->Render();
			if (finishStep >= 2)
				cardVisuals[card]->Draw(576, 192 - 32, 0, 0);
		}

		if (alpha > 0)
		{
			auto conf = Game::GetInstance().GetGlobalConfigs();
			Game::GetInstance().DrawTexture(0, 0, 0, 0, mask, 0, 0, conf.screenWidth, conf.screenHeight, (int)alpha);
		}
	}
	break;
	case GameState::Die:
	case GameState::Menu:
	case GameState::GameOver:
	{
		if (alpha > 0)
		{
			auto conf = Game::GetInstance().GetGlobalConfigs();
			Game::GetInstance().DrawTexture(0, 0, 0, 0, mask, 0, 0, conf.screenWidth, conf.screenHeight, (int)alpha);
		}
	}
	break;
	case GameState::Run:
	{
		if (alpha > 0)
		{
			auto conf = Game::GetInstance().GetGlobalConfigs();
			Game::GetInstance().DrawTexture(0, 0, 0, 0, mask, 0, 0, conf.screenWidth, conf.screenHeight, (int)alpha);
		}
		pause->Render();
	}
	break;
	case GameState::Ready:
	{
		if (transition > 0)
		{
			auto graphics = Game::GetInstance();
			auto conf = Game::GetInstance().GetGlobalConfigs();

			if (transition >= 2)
			{
				graphics.DrawTexture(0, 0, 0, 0, mask, 0, 0, conf.screenWidth, conf.screenHeight, (int)alpha);
			}
			else
			{
				graphics.DrawTexture(wipePosition.x - conf.screenWidth, 0, 0, 0, mask, 0, 0, conf.screenWidth, conf.screenHeight, (int)alpha);
				graphics.DrawTexture(conf.screenWidth - wipePosition.x, 0, 0, 0, mask, 0, 0, conf.screenWidth, conf.screenHeight, (int)alpha);
				graphics.DrawTexture(0, wipePosition.y - conf.screenHeight, 0, 0, mask, 0, 0, conf.screenWidth, conf.screenHeight, (int)alpha);
				graphics.DrawTexture(0, conf.screenHeight - wipePosition.y, 0, 0, mask, 0, 0, conf.screenWidth, conf.screenHeight, (int)alpha);
			}
		}
	}
	break;
	}
}

void MainCanvas::OnSceneUnload()
{
	// gameState = GameState::Unload;
	Game::GetInstance().SetTimeScale(1);
	ResetTimer();
	player = nullptr;
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
	DebugOut(L"GAME START\n");
	ResetTimer();
	gameState = GameState::Run;
}

void MainCanvas::GetGameReady()
{
	courseClear->SetContent("");
	reward->SetContent("");
	gameState = GameState::Ready;
	hud->SetActive(true);
}

void MainCanvas::LoseGame()
{
	DebugOut(L"LOSE\n");
	finishTimer = 0;
	finishStep = 0;

	if (timeFreeze)
	{
		pSwitchTimer = 0;
		timeFreeze = false;
	}

	gameState = GameState::Die;

	auto data = Game::GetInstance().GetData();
	auto temp = data->GetWorldMapTempData();
	temp.status = GameplayStatus::Lose;
	data->SetWorldMapTempData(temp);
}

void MainCanvas::FinishGame(int card)
{
	finishTimer = 0;
	finishStep = 0;

	if (timeFreeze)
	{
		pSwitchTimer = 0;
		timeFreeze = false;
	}

	gameState = GameState::Finish;
	this->card = card;
}

void MainCanvas::StartTransition()
{
	transition = 1;
	DebugOut(L"Canvas Start transition: %d\n", alpha);
}

void MainCanvas::StartMenu()
{
	gameState = GameState::Menu;
	hud->SetActive(false);
	DebugOut(L"Gamestate: %d\n", gameState == GameState::Menu ? 1 : 0);
}

void MainCanvas::CloseMenu()
{
	finishStep = 1;
}

void MainCanvas::WorldIntro(bool restartWorld)
{
	if (gameState != GameState::Menu && !restartWorld) return;

	gameOverDialog->SetActive(false);

	OpenWorldDialog();
	gameState = GameState::WorldStart;
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

bool MainCanvas::IsIngame()
{
	return this->gameState == GameState::Run;
}

bool MainCanvas::IsDialogOpening()
{
	return dialogOpening;
}

void MainCanvas::OpenWorldDialog()
{
	worldDialog->SetActive(true);
	dialogOpening = true;
}

void MainCanvas::CloseWorldDialog()
{
	worldDialog->SetActive(false);
	dialogOpening = false;
}

void MainCanvas::OpenGameOverDialog()
{
	gameOverDialog->SetActive(true);
	dialogOpening = true;
}

void MainCanvas::CloseGameOverDialog()
{
	gameOverDialog->SetActive(false);
	dialogOpening = false;
}

void MainCanvas::OnGameOver()
{
	OpenGameOverDialog();
	gameState = GameState::GameOver;
}

void MainCanvas::RestartWorld()
{
	// StartMenu();
	gameState = GameState::Menu;
	Game::GetInstance().GetData()->ResetData();
	StartTransition();
}

void MainCanvas::SetTargetScene(std::string id)
{
	targetSceneID = id;
}

void MainCanvas::GameReady()
{
	switch (transition)
	{
	case 1:
	{
		auto config = Game::GetInstance().GetGlobalConfigs();
		int duration = 700; // 0.7s
		int delay = 300;
		int dx = config.screenWidth * 0.35f;
		int dy = config.screenHeight * 0.5f;
		float vx = (float)dx / (float)duration;
		float vy = (float)dy / (float)duration;
		wipePosition.x += vx * Game::DeltaTime();
		wipePosition.y += vy * Game::DeltaTime();
		alpha = 255;

		transTimer += Game::DeltaTime();
		if (transTimer > duration + delay)
		{
			transTimer = 0;
			auto gameplayScene = new ExampleScene();
			std::string path = Game::GetInstance().GetSourcePathOf(CATEGORY_SCENE, targetSceneID);
			// OutputDebugStringW(ToLPCWSTR(path + "\n"));
			gameplayScene->SetFilePath(path);
			transition = 10;
			Game::GetInstance().GetService<SceneManager>()->LoadScene(gameplayScene);
		}
	}
	break;
	case 2:
	{
		alpha -= (255.0f / (float)TRANSITION_TIME) * Game::DeltaTime();
		if (alpha <= 0)
		{
			alpha = 0;
			wipePosition = VectorZero();
			transition = 3;
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
	time -= dtScaled * 1.0f;

	// Time's Up 
	if (time <= 1)
	{
		time = 0;
		player->SwitchToState("Die");
	}

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
	case 10:
	{
		alpha -= (255.0f / (float)TRANSITION_TIME) * dt;
		if (alpha <= 1)
		{
			alpha = 0;
			if (transition == 10)
				wipePosition = VectorZero();
			else
				mario->WarpOut();
			transition = 3;
		}
	}
	break;
	case 3:
	{
		alpha = 0;
		transition = 0;
		Game::GetInstance().SetTimeScale(1);
	}
	break;
	}
}

void MainCanvas::GameFinish()
{
	hud->SetPowerMeter(0);
	// DebugOut(L"Game state\n");

	auto dt = Game::DeltaTime();

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
				alpha = 0;
				finishTimer = 0;
				finishStep = 4;
				hud->StopBlinkingLastCard();
			}
		}
	}
	break;
	case 4:
	{
		finishTimer += dt;
		if (finishTimer > 1000)
		{
			transition = 1;
			finishTimer = 0;
			finishStep = 5;
			auto data = Game::GetInstance().GetData();
			auto temp = data->GetWorldMapTempData();
			temp.status = GameplayStatus::Victory;
			data->SetWorldMapTempData(temp);
		}
	}
	break;
	}

	switch (transition)
	{
	case 1:
	{
		alpha += (255.0f / (float)1000) * dt;
		if (alpha >= 255)
		{
			alpha = 255;
			transition = 2;
			GetGameReady();
			finishTimer = 0;
			finishStep = 0;
			Game::GetInstance().GetService<SceneManager>()->LoadScene(new WorldMapScene());
		}
	}
	break;
	}
}

void MainCanvas::GameLose()
{
	auto dt = Game::DeltaTime();
	switch (finishStep)
	{
	case 0:
	{
		finishTimer += dt;
		if (finishTimer > 3000)
		{
			finishTimer = 0;
			StartTransition();
			finishStep = 1;
		}
	}
	break;
	}

	switch (transition)
	{
	case 1:
	{
		alpha += (255.0f / (float)1000) * dt;
		if (alpha >= 255)
		{
			alpha = 255;
			transition = 2;
			GetGameReady();
			finishTimer = 0;
			finishStep = 0;
			Game::GetInstance().GetService<SceneManager>()->LoadScene(new WorldMapScene());

			auto data = Game::GetInstance().GetData();
			if (data->life == 0)
				OnGameOver();
			data->ModifyLife(-1, true);
		}
	}
	break;
	}
}

void MainCanvas::GameMenu()
{
	auto dt = Game::DeltaTime();

	switch (finishStep)
	{
	case 1:
	{
		finishTimer += dt;
		if (finishTimer > 1000)
		{
			finishTimer = 0;
			StartTransition();
			finishStep = 0;
		}
	}
	break;
	}

	switch (transition)
	{
	case 1:
	{
		alpha += (255.0f / (float)500) * dt;
		if (alpha >= 255)
		{
			alpha = 255;
			transition = 2;
			finishTimer = 0;
			finishStep = 0;
			hud->SetActive(true);
			Game::GetInstance().GetService<SceneManager>()->LoadScene(new WorldMapScene());
		}
	}
	break;
	case 2:
	{
		alpha -= (255.0f / (float)TRANSITION_TIME) * Game::DeltaTime();
		if (alpha <= 1)
		{
			alpha = 0;
			transition = 0;
		}
	}
	break;
	}
}

void MainCanvas::GameWorldStart()
{
	auto dt = Game::DeltaTime();
	finishTimer += dt;
	if (finishTimer > 2000)
	{
		finishTimer = 0;
		CloseWorldDialog();
		GetGameReady();
	}

	switch (transition)
	{
	case 2:
	{
		alpha -= (255.0f / (float)TRANSITION_TIME) * Game::DeltaTime();
		if (alpha <= 1)
		{
			alpha = 0;
			transition = 3;
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

void MainCanvas::GameOver()
{
	Game::SetTimeScale(0);
	switch (transition)
	{
	case 1:
	{
		alpha += (255.0f / (float)TRANSITION_TIME) * Game::DeltaTime();
		if (alpha >= 255)
		{
			alpha = 255;
			transition = 2;
			Game::GetInstance().GetService<SceneManager>()->LoadScene(new MainMenuScene());
		}
	}
	break;
	case 2:
	{
		alpha -= (255.0f / (float)TRANSITION_TIME) * Game::DeltaTime();
		if (alpha <= 1)
		{
			alpha = 0;
			transition = 0;
		}
	}
	break;
	}
}
