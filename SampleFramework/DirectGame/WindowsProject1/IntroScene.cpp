#include "IntroScene.h"
#include "SceneAnimator.h"
#include "MainCanvas.h"
#include "Game.h"
#include "SolidBox.h"

void IntroScene::Load()
{
	this->id = "IntroScene";
	this->filePath = Game::GetInstance().GetSourcePathOf(CATEGORY_SCENE, SC_INTRO);

	needSpatialPartition = false;

	TiXmlDocument document(filePath.c_str());
	if (document.LoadFile() == false)
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return;
	}

	TiXmlElement* root = document.RootElement();
	int r, g, b;
	root->QueryIntAttribute("bgcolor_r", &r);
	root->QueryIntAttribute("bgcolor_g", &g);
	root->QueryIntAttribute("bgcolor_b", &b);
	Game::GetInstance().SetClearColor(D3DCOLOR_XRGB(r, g, b));
	for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		string name = element->Attribute("name");

		if (name.compare("Camera") == 0)
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

	auto animator = Instantiate<SceneAnimator>();
	AddObject(animator);

	auto topSolid = Instantiate<SolidBox>();
	topSolid->GetColliders()->at(0)->SetBoxSize(Vector2(824, 32));
	topSolid->SetPosition(Vector2(412, 16));
	AddObject(topSolid);

	auto bottomSolid = Instantiate<SolidBox>();
	bottomSolid->GetColliders()->at(0)->SetBoxSize(Vector2(824, 32));
	bottomSolid->SetPosition(Vector2(412, 642 + 16));
	AddObject(bottomSolid);

	loaded = true;

	auto mainCanvas = dynamic_cast<MainCanvas*>(Canvas::GetCanvas("main"));
	if (mainCanvas != nullptr) mainCanvas->StartMenu();
}
