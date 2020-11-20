 #include "Scene.h"
#include "Game.h"
#include "CMario.h"
#include "tinyxml.h"
#include "PlayerController.h"

Scene::Scene()
{
	objects = new vector<GameObject>();
	loaded = false;
}

Scene::Scene(std::string filePath)
{
	this->filePath = filePath;
	loaded = false;
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

	loaded = true;
}

void Scene::Unload()
{
	loaded = false;
	for (auto object : *objects)
		delete object;
	objects->clear();
	delete objects;
}

void Scene::Update()
{
	if (loaded == false) return;

	if (!(objects == nullptr || objects->size() == 0))
	{
		for (std::vector<GameObject>::iterator obj = objects->begin(); obj != objects->end(); ++obj)
		{
			if (*obj == nullptr) continue;
			if ((*obj)->IsDestroyed()) continue;
			if ((*obj)->IsEnabled() == false) continue;
			if (!mainCamera->PointInsideCameraView((*obj)->GetTransform().Position, 48 * 6))
				continue;

			if ((*obj)->GetRigidbody()->IsDynamic()) (*obj)->PhysicsUpdate(objects);
		}

		for (std::vector<GameObject>::iterator obj = objects->begin(); obj != objects->end(); ++obj)
		{
			if (*obj == nullptr) continue;
			if ((*obj)->IsDestroyed()) continue;
			if ((*obj)->IsEnabled() == false) continue;
			if (!mainCamera->PointInsideCameraView((*obj)->GetTransform().Position, 48 * 6))
				continue;

			(*obj)->Update();
		}

		for (std::vector<GameObject>::iterator obj = objects->begin(); obj != objects->end(); ++obj)
		{
			if (*obj == nullptr) continue;
			if ((*obj)->IsDestroyed()) continue;
			if ((*obj)->IsEnabled() == false) continue;
			if (!mainCamera->PointInsideCameraView((*obj)->GetTransform().Position, 48 * 6))
				continue;

			(*obj)->LateUpdate();
		}
	}

	if (mainCamera != nullptr) mainCamera->Update();
}

void Scene::Render()
{
	if (loaded == false) return;

	if (mainCamera != nullptr) mainCamera->Render();

	if (!(objects == nullptr || objects->size() == 0))
	{
		for (std::vector<GameObject>::iterator obj = objects->begin(); obj != objects->end(); ++obj)
		{
			if (*obj == nullptr) continue;
			if ((*obj)->IsDestroyed()) continue;
			if ((*obj)->IsEnabled() == false) continue;
			if (!mainCamera->PointInsideCameraView((*obj)->GetTransform().Position, 48 * 6))
				continue;

			(*obj)->PreRender();
		}

		for (std::vector<GameObject>::iterator obj = objects->begin(); obj != objects->end(); ++obj)
		{
			if (*obj == nullptr) continue;
			if ((*obj)->IsDestroyed()) continue;
			if ((*obj)->IsEnabled() == false) continue;
			if (!mainCamera->PointInsideCameraView((*obj)->GetTransform().Position, 48 * 6))
				continue;

			(*obj)->Render(-mainCamera->GetPosition());
		}
	}
}

void Scene::CleanDestroyedObjects()
{
	if (destroyed.size() > 0)
	{
		for (auto x : destroyed)
		{
			if (x)
			{
				objects->erase(std::remove(objects->begin(), objects->end(), x));
				delete x;
				x = nullptr;
			}
		}
		destroyed.clear();
	}
}

void Scene::AddObject(GameObject gameObject)
{
	if (find(objects->begin(), objects->end(), gameObject) != objects->end()) return;

	objects->push_back(gameObject);
	std::sort(objects->begin(), objects->end(), Scene::Comparator);
}

void Scene::RemoveObject(GameObject gameObject)
{
	// gameObject->SetActive(false);
	/*if (find(objects->begin(), objects->end(), gameObject) != objects->end())
		objects->erase(std::remove(objects->begin(), objects->end(), gameObject), objects->end());*/
	gameObject->SetDestroyed();
	destroyed.push_back(gameObject);
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
