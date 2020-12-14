#include "ExampleScene.h"
#include "Game.h"
#include "Consts.h"
#include "CMario.h"
#include "SolidBox.h"
#include "MainCanvas.h"

void ExampleScene::Load()
{
	DebugOut(L"Example scene\n");
	this->id = "ExampleScene";
	this->filePath = Game::GetInstance().GetSourcePathOf(CATEGORY_SCENE, SC_WOLRD_1_1);
	Scene::Load();
	auto mainCanvas = dynamic_cast<MainCanvas*>(Canvas::GetCanvas("main"));
	if (mainCanvas != nullptr) mainCanvas->StartGame();
}
