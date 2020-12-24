#include "WorldDialog.h"
#include "Game.h"

void WorldDialog::Awake()
{
	background = Game::GetInstance().GetService<SpriteManager>()->Get("spr-dialog-mario-0");
	
	auto font = Game::GetInstance().GetGlobalFont();
	
	life = new Text();
	world = new Text();

	life->SetFont(font);
	world->SetFont(font);
}

void WorldDialog::Start()
{
	world->SetPosition(rectTransform.Position + Vector2(244, 41));
	life->SetPosition(rectTransform.Position + Vector2(318, 107));
}

void WorldDialog::PreRender()
{
	auto data = Game::GetInstance().GetData();
	world->SetPosition(rectTransform.Position + Vector2(244, 41));
	life->SetPosition(rectTransform.Position + Vector2(318, 107));

	life->SetContent(to_string(data->life));
	world->SetContent(to_string(data->world));
}

void WorldDialog::Render()
{
	if (!IsActive()) return;
	background->Draw(rectTransform.Position.x, rectTransform.Position.y, 0, 0);
	life->Render();
	world->Render();
}
