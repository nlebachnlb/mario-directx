#include "MainCanvas.h"
#include "Game.h"
#include "Consts.h"

void MainCanvas::Awake()
{
	hud = new HudPanel();
	hud->Initialize();
	AddUIElement(hud);

	mask = Game::GetInstance().GetService<TextureManager>()->GetTexture(TEXTURE_BOX);
}

void MainCanvas::Start()
{

}

void MainCanvas::Render()
{
	Game::GetInstance().DrawTexture(0, 594, 0, 0, mask, 0, 0, 824, 150);
	Canvas::Render();
}
