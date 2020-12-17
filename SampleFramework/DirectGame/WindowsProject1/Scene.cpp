#include "Scene.h"
#include "Game.h"
#include "CMario.h"
#include "tinyxml.h"
#include "PlayerController.h"
#include "EffectPool.h"
#include <algorithm>
#include "Canvas.h"

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
	delete objects;
	objects = nullptr;
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
			int sx = 0, sy = 0;
			element->QueryIntAttribute("pos_x", &sx);
			element->QueryIntAttribute("pos_y", &sy);
			Vector2 startPosition(sx, sy);

			objMario = Instantiate<PlayerController>();
			objMario->RegisterToScene(this);
			objMario->SetPosition(startPosition);
			objMario->InitStates();
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
			if (objMario != nullptr) camera->SetTarget(objMario);
			camera->SetBoundary(startBoundary.boundary);
			camera->LockBoundary();
			SetMainCamera(camera);
		}
	}
	std::sort(objects->begin(), objects->end(), Scene::Comparator);

	loaded = true;
}

void Scene::Init()
{
	auto gmap = Game::GetInstance().GetService<GameMap>();
	auto spawner = gmap->GetSpawnerManager();

	if (spawner != nullptr)
	{
		auto fxPool = spawner->GetService<EffectPool>();
		fxPool->Initialization();
	}

	if (gmap != nullptr) gmap->LoadEnemy();

	ProcessInstantiateRequests();
	for (auto o : *objects)
		o->OnEnabled();
#pragma region Debug Logs
	/*
	DebugOut(L"\n");
	for (auto o : *objects)
		DebugOut(L"%f-", o->GetTransform().Position.x),
		o->OnEnabled();
	DebugOut(L"\n");*/
#pragma endregion
}

void Scene::Unload()
{
	loaded = false;

	Canvas::OnSceneUnloadedEvent();

	/*for (auto o : *objects)
		delete o;*/

	for (auto o : *objects) 
		Destroy(o);

	/*objects->clear();
	delete objects;
	objects = nullptr;*/

	if (mainCamera != nullptr)
	{
		delete mainCamera;
		mainCamera = nullptr;
	}

	Game::GetInstance().GetService<GameMap>()->Unload();
}

void Scene::Update()
{
	if (loaded == false) return;

	for (auto o : updated)
		if (o->IsEnabled()) o->PhysicsUpdate(objects);

	for (auto o : updated)
		if (o->IsEnabled()) o->Update();

	for (auto o : updated)
		if (o->IsEnabled()) o->LateUpdate();

	if (mainCamera != nullptr) mainCamera->Update();
}

void Scene::Render()
{
	if (loaded == false) return;

	for (auto o : updated)
		o->PreRender();

	if (mainCamera != nullptr) mainCamera->Render(updated);
}

void Scene::CleanDestroyedObjects()
{
	//if (loaded == false) return;

	if (destroyed.size() > 0)
	{
		for (auto o : destroyed)
			Remove(o);

		for (auto o : destroyed)
		{
			delete o;
			o = nullptr;
		}

		destroyed.clear();
	}
}

void Scene::ProcessInstantiateRequests()
{
	if (loaded == false) return;

	if (instantiated.size() > 0)
	{
		for (auto o : instantiated)
		{
			if (objects->empty()) { objects->push_back(o); continue; }

			// Binary search the approriate position
			auto pos = std::lower_bound(objects->begin(), objects->end(), o, Scene::Comparator);
			objects->insert(pos, o);
		}
		instantiated.clear();
	}
}

void Scene::UpdateActiveObjects()
{
	updated.clear();
	if (objects != nullptr)
	{
		for (auto o : *objects)
		{
			if (o == nullptr) continue;
			if (o->IsDestroyed()) continue;
			if (o->IsEnabled() == false) continue;
			if (!TagUtils::MarioTag(o->GetTag()))
			{
				if (TagUtils::StaticTag(o->GetTag()))
				{
					if (!mainCamera->RectInsideCameraView(o->GetColliders()->at(0)->GetBoundingBox()))
					{
						o->SetOffScreen(true);
						continue;
					}
				}
				else
				{
					if (!mainCamera->PointInsideCameraView(o->GetTransform().Position, 48 * 3))
					{
						o->SetOffScreen(true);
						continue;
					}
				}
			}

			o->SetOffScreen(false);
			auto pos = std::lower_bound(updated.begin(), updated.end(), o, Scene::Comparator);
			if (pos != updated.end()) updated.insert(pos, o);
			else updated.push_back(o);
		}
	}
}

void Scene::SetFilePath(std::string path)
{
	this->filePath = path;
}

void Scene::AddObject(GameObject gameObject)
{
	if (find(objects->begin(), objects->end(), gameObject) != objects->end()) return;

	// objects->push_back(gameObject);
	instantiated.push_back(gameObject);
	// std::sort(objects->begin(), objects->end(), Scene::Comparator);
}

void Scene::RemoveObject(GameObject gameObject)
{
	// Mark object as destroyed object, wait for cleaning phase
	gameObject->SetDestroyed();
	destroyed.push_back(gameObject);
}

std::vector<GameObject>* Scene::GetSceneObjects()
{
	return objects;
}

std::vector<GameObject>& Scene::GetActiveObjects()
{
	return updated;
}

void Scene::SetMainCamera(Camera* camera)
{
	this->mainCamera = camera;
}

Camera* Scene::GetMainCamera()
{
	return this->mainCamera;
}

void Scene::Remove(GameObject go)
{
	auto index = find(objects->begin(), objects->end(), go);
	if (index != objects->end())
		objects->erase(index);
}
