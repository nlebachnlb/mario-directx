#include "MainCanvas.h"

void MainCanvas::Awake()
{
	hud = new HudPanel();
	hud->Initialize();
	AddUIElement(hud);
}

void MainCanvas::Start()
{

}
