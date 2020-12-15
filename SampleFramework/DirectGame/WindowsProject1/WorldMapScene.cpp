#include "WorldMapScene.h"
#include "Game.h"
#include "MainCanvas.h";

void WorldMapScene::Load()
{
	DebugOut(L"World map\n");
	this->id = "WorldMapScene";
	this->filePath = Game::GetInstance().GetSourcePathOf(CATEGORY_SCENE, SC_OVERWORLD_MAP_1);
	Scene::Load();

	mainCamera->SetRenderOffset(Vector2(28, 0));
	/*auto mainCanvas = dynamic_cast<MainCanvas*>(Canvas::GetCanvas("main"));
	if (mainCanvas != nullptr) mainCanvas->StartGame();*/
}
