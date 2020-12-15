#include "WorldMapScene.h"
#include "Game.h"
#include "MainCanvas.h";


void WorldMapScene::Load()
{
	DebugOut(L"World map\n");
	this->id = "WorldMapScene";
	this->filePath = Game::GetInstance().GetSourcePathOf(CATEGORY_SCENE, SC_OVERWORLD_MAP_1);
	
	TiXmlDocument document(filePath.c_str());
	if (document.LoadFile() == false)
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return;
	}

	objMario = nullptr;

	TiXmlElement* root = document.RootElement();
	int r, g, b;
	root->QueryIntAttribute("bgcolor_r", &r);
	root->QueryIntAttribute("bgcolor_g", &g);
	root->QueryIntAttribute("bgcolor_b", &b);
	Game::GetInstance().SetClearColor(D3DCOLOR_XRGB(r, g, b));
	for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		string name = element->Attribute("name");

		if (name.compare("Map") == 0)
		{
			auto source = element->Attribute("source");

			auto gameMap = Game::GetInstance().GetService<GameMap>();
			gameMap->Load(source, true);

			auto mapObjects = gameMap->GetGameObjects();
			for (auto obj : mapObjects)
				AddObject(obj);
		}
		else if (name.compare("MarioLocator") == 0)
		{
			objMario = Instantiate<MarioLocator>();
			objMario->SetActive(false);
			AddObject(objMario);
		}
		else if (name.compare("Camera") == 0)
		{
			Camera* camera = new Camera();
			int start;
			element->QueryIntAttribute("start", &start);

			for (auto bound = element->FirstChildElement(); bound != nullptr; bound = bound->NextSiblingElement())
			{
				int id;
				BoundarySet bSet;
				bound->QueryIntAttribute("id", &id);
				bound->QueryFloatAttribute("pos_x", &bSet.position.x);
				bound->QueryFloatAttribute("pos_y", &bSet.position.y);
				bound->QueryFloatAttribute("left", &bSet.boundary.left);
				bound->QueryFloatAttribute("top", &bSet.boundary.top);
				bound->QueryFloatAttribute("right", &bSet.boundary.right);
				bound->QueryFloatAttribute("bottom", &bSet.boundary.bottom);
				camera->AddBoundarySet(id, bSet);
			}

			BoundarySet startBoundary = camera->GetBoundarySet(start);
			camera->SetPosition(startBoundary.position);
			camera->SetTarget(nullptr);
			camera->SetBoundary(startBoundary.boundary);
			camera->LockBoundary();
			SetMainCamera(camera);
		}
	}
	std::sort(objects->begin(), objects->end(), Scene::Comparator);

	loaded = true;

	mainCamera->SetRenderOffset(Vector2(28, 0));
	DebugOut(L"WORLD: %f, %f\n", mainCamera->GetPosition().x, mainCamera->GetPosition().y);

	objMario->SetCurrentNode(0);
	objMario->SetActive(true);

	/*auto mainCanvas = dynamic_cast<MainCanvas*>(Canvas::GetCanvas("main"));
	if (mainCanvas != nullptr) mainCanvas->StartGame();*/
}

MarioLocator* WorldMapScene::GetMarioLocator()
{
	return objMario;
}
