#include "MainCanvas.h"
#include "Game.h"
#include "Consts.h"
#include "Mathf.h"

void MainCanvas::Awake()
{
	hud = new HudPanel();
	hud->Initialize();
	AddUIElement(hud);

	mask = Game::GetInstance().GetService<TextureManager>()->GetTexture(TEXTURE_BOX);
}

void MainCanvas::Start()
{
	hud->SetWorld(1);
	hud->SetScore(0);
	hud->SetCoin(0);
	hud->SetTimer(0);
	hud->SetLife(4);
}

void MainCanvas::Update()
{
	if (gameStarted)
	{
		if (player == nullptr)
		{
			auto obj = Game::GetInstance().FindGameObjectWithTag(ObjectTags::Player);
			if (obj != nullptr) player = static_cast<PlayerController*>(obj);
			else return;
		}

		auto mario = player->GetMario();
		auto vel = mario->GetRigidbody()->GetVelocity();

		// Convert to percents
		//double percent = Mathf::Abs(vel.x) / MARIO_RUN_SPEED;
		auto percent = mario->GetPMeter();
		int powerLevel = (int)(percent);
		hud->SetPowerMeter(powerLevel);
	}
}

void MainCanvas::PreRender()
{
	hud->SetLife(gameData->life);
	hud->SetWorld(gameData->world);
	hud->SetScore(gameData->score);
	hud->SetCoin(gameData->coin);

	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	time -= dt * 1.5f;

	hud->SetTimer((int)((float)time * 0.001f));
	hud->PreRender();
}

void MainCanvas::Render()
{
	Game::GetInstance().DrawTexture(0, 594, 0, 0, mask, 0, 0, 824, 150);
	Canvas::Render();
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
	gameStarted = true;
	ResetTimer();
}
