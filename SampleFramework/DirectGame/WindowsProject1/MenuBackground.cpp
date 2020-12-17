#include "MenuBackground.h"
#include "Game.h"
#include "MainCanvas.h"

void MenuBackground::Awake()
{
	auto spr = Game::GetInstance().GetService<SpriteManager>();
	arrow = spr->Get("spr-menu-arrow-0");
	currentOption = 0;
}

void MenuBackground::AddElement(DrawInfo info)
{
	auto spr = Game::GetInstance().GetService<SpriteManager>();

	auto sprite = spr->Get(info.spriteID);
	if (sprite != nullptr)
	{
		elements.push_back(info);
		sprites.push_back(sprite);
	}
}

void MenuBackground::AddOption(Vector2 opt)
{
	options.push_back(opt);
}

void MenuBackground::LateUpdate()
{
	if (blink)
	{
		timer += Game::DeltaTime();
		if (timer > MENU_BLINK_TIME) timer = 0;
	}
}

void MenuBackground::Render(Vector2 translation)
{
	for (int i = 0; i < elements.size(); ++i)
		sprites.at(i)->Draw(
			elements.at(i).position.x, elements.at(i).position.y,
			elements.at(i).pivot.x, elements.at(i).pivot.y);

	if (currentOption < options.size())
	{
		if ((blink && timer > MENU_BLINK_TIME * 0.5f) || !blink)
			arrow->Draw(options.at(currentOption).x, options.at(currentOption).y);
	}
}

void MenuBackground::OnKeyDown(int keyCode)
{
	if (blink) return;

	switch (keyCode)
	{
	case DIK_DOWN:
		currentOption = (currentOption + 1) % options.size();
		break;
	case DIK_UP:
		currentOption = currentOption - 1 < 0 ? options.size() - 1 : currentOption - 1;
		break;
	case DIK_RETURN:
	{
		if (currentOption == 0)
		{
			blink = true;
			Game::GetInstance().SetTimeScale(0);
			auto mainCanvas = static_cast<MainCanvas*>(Canvas::GetCanvas("main"));
			if (mainCanvas != nullptr) mainCanvas->CloseMenu();
		}
	}
	break;
	}
}
