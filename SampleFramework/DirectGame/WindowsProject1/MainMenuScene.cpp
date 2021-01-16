#include "MainMenuScene.h"
#include "Game.h"
#include "MenuBackground.h"
#include "MainCanvas.h"
#include "MenuVersionFX.h"
#include "MenuKoopa.h"

void MainMenuScene::Load()
{
	DebugOut(L"Main Menu\n");
	this->id = "MainMenuScene";
	this->filePath = Game::GetInstance().GetSourcePathOf(CATEGORY_SCENE, SC_MENU);
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

		if (name.compare("MenuBackground") == 0)
		{
			auto obj = Instantiate<MenuBackground>();

			for (auto elm = element->FirstChildElement("Sprite"); elm != nullptr; elm = elm->NextSiblingElement("Sprite"))
			{
				int x, y, px, py;
				std::string id = elm->Attribute("id");
				elm->QueryIntAttribute("x", &x);
				elm->QueryIntAttribute("y", &y);

				if (id.compare("menu-option") == 0)
				{
					obj->AddOption(Vector2(x, y));
				}
				else
				{
					elm->QueryIntAttribute("px", &px);
					elm->QueryIntAttribute("py", &py);
					obj->AddElement({ id, Vector2(x, y), Vector2(px, py) });
				}
			}

			AddObject(obj);

			for (auto elm = element->FirstChildElement("Object"); elm != nullptr; elm = elm->NextSiblingElement("Object"))
			{
				int x, y;
				std::string name = elm->Attribute("name");
				elm->QueryIntAttribute("x", &x);
				elm->QueryIntAttribute("y", &y);

				if (name.compare("menu-version") == 0)
				{
					auto object = Instantiate<MenuVersionFX>();
					object->SetPosition(Vector2(x, y));
					AddObject(object);
				}
				else if (name.compare("menu-koopa") == 0)
				{
					auto object = Instantiate<MenuKoopa>();
					object->SetPosition(Vector2(x, y));
					AddObject(object);
				}
			}
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

	auto mainCanvas = dynamic_cast<MainCanvas*>(Canvas::GetCanvas("main"));
	if (mainCanvas != nullptr) mainCanvas->StartMenu();
}
