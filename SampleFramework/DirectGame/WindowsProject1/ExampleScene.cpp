#include "ExampleScene.h"
#include "Game.h"
#include "Consts.h"
#include "CMario.h"
#include "SolidBox.h"

void ExampleScene::Load()
{
	this->id = "ExampleScene";
	this->filePath = Game::GetInstance().GetSourcePathOf(CATEGORY_SCENE, SC_WOLRD_1_1);
	Scene::Load();
}
