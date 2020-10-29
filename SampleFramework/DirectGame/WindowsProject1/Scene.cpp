#include "Scene.h"
#include "Game.h"
#include "CMario.h"
#include "tinyxml.h"
#include "PlayerController.h"

Scene::Scene()
{
	objects = new vector<GameObject>();
}

Scene::Scene(std::string filePath)
{
	this->filePath = filePath;
}

Scene::~Scene()
{
	if (mainCamera != nullptr) delete mainCamera;
}

void Scene::Load()
{
	TiXmlDocument document(filePath.c_str());
	if (document.LoadFile() == false)
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return;
	}

	PlayerController* objMario = nullptr;

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
		else if (name.compare("Player") == 0)
		{
			Vector2 startPosition;
			element->QueryFloatAttribute("pos_x", &startPosition.x);
			element->QueryFloatAttribute("pos_y", &startPosition.y);

			objMario = Instantiate<PlayerController>();
			objMario->RegisterToScene(this);
			objMario->SetPosition(startPosition);
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
			if(objMario != nullptr) camera->SetTarget(objMario);
			camera->SetBoundary(startBoundary.boundary);
			SetMainCamera(camera);
		}
	}
	std::sort(objects->begin(), objects->end(), Scene::Comparator);
}

void Scene::Unload()
{
	for (auto object : *objects)
		delete object;
	objects->clear();
	delete objects;
}

void Scene::Update()
{
	if (!(objects == nullptr || objects->size() == 0))
	{
		for (std::vector<GameObject>::iterator obj = objects->begin(); obj != objects->end(); ++obj)
		{
			if ((*obj)->IsEnabled() == false) continue;
			(*obj)->PhysicsUpdate(objects);
			(*obj)->Update();
			(*obj)->LateUpdate();
		}
	}

	if (mainCamera != nullptr) mainCamera->Update();
}

void Scene::Render()
{
	if (mainCamera != nullptr) mainCamera->Render();

	if (!(objects == nullptr || objects->size() == 0))
	{
		for (std::vector<GameObject>::iterator obj = objects->begin(); obj != objects->end(); ++obj)
		{
			if ((*obj)->IsEnabled() == false) continue;
			Vector2 translation = -mainCamera->GetPosition();
			(*obj)->PreRender();
			(*obj)->Render(translation);
		}
	}
}

void Scene::AddObject(GameObject gameObject)
{
	objects->push_back(gameObject);
	// DebugOut(L"objects' length: %d", objects->size());
	// push_heap(objects->begin(), objects->end(), Comparator);
}

void Scene::RemoveObject(GameObject gameObject)
{
	auto remObj = find(objects->begin(), objects->end(), gameObject);
	if (remObj != objects->end())
	{
		objects->erase(remObj);
		// pop_heap(objects->begin(), objects->end(), Comparator);
	}
}

std::vector<GameObject>* Scene::GetSceneObjects()
{
	return objects;
}

void Scene::SetMainCamera(Camera* camera)
{
	this->mainCamera = camera;
}

Camera* Scene::GetMainCamera()
{
	return this->mainCamera;
}
