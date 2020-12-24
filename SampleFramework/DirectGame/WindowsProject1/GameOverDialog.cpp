#include "GameOverDialog.h"
#include "Game.h"
#include "MainCanvas.h"

void GameOverDialog::Awake()
{
	background = Game::GetInstance().GetService<SpriteManager>()->Get("spr-dialog-gameover-0");
	arrow = Game::GetInstance().GetService<SpriteManager>()->Get("spr-dialog-arrow-0");

	options.push_back(Vector2(133, 118));
	options.push_back(Vector2(133, 144));
}

void GameOverDialog::Start()
{
	option = 0;
}

void GameOverDialog::PreRender()
{
}

void GameOverDialog::Render()
{
	if (!IsActive()) return;
	background->Draw(rectTransform.Position.x, rectTransform.Position.y, 0, 0);
	auto pos = rectTransform.Position + options.at(option);
	arrow->Draw(pos.x, pos.y, 0, 0);
}

void GameOverDialog::OnKeyDown(int keyCode)
{
	switch (keyCode)
	{
	case DIK_UP: option = option - 1 < 0 ? options.size() - 1 : option - 1; break;
	case DIK_DOWN: option = (option + 1) % options.size(); break;
	case DIK_RETURN: 
	{
		switch (option)
		{
		case 0:
		{
			auto canvas = static_cast<MainCanvas*>(Canvas::GetCanvas("main"));
			canvas->RestartWorld();
		}
		break;
		case 1:
		{
			auto canvas = static_cast<MainCanvas*>(Canvas::GetCanvas("main"));
			canvas->CloseGameOverDialog();
			Game::GetInstance().GetData()->ResetData();
			canvas->StartTransition();
		}
		break;
		}
	}
	break;
	}
}
