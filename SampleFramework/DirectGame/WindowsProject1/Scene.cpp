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

void Scene::Init()
{
	for (auto o : *objects)
		o->OnEnabled();
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
	updated.clear();

	if (objects != nullptr)
	{
		for (auto o : *objects)
		{
			if (o == nullptr) continue;
			if (o->IsDestroyed()) continue;
			if (o->IsEnabled() == false) continue;
			if (!mainCamera->PointInsideCameraView(o->GetTransform().Position, 48 * 6))
				continue;

			updated.push_back(o);
		}
	}

	for (auto o : updated)
		o->PhysicsUpdate(objects);

	for (auto o : updated)
		o->Update();

	for (auto o : updated)
		o->LateUpdate();
	
	if (mainCamera != nullptr) mainCamera->Update();
}

void Scene::Render()
{
	if (loaded == false) return;

	if (mainCamera != nullptr) mainCamera->Render();

	for (auto o : updated)
		o->PreRender();

	for (auto o : updated)
		o->Render(-mainCamera->GetPosition());
}

void Scene::CleanDestroyedObjects()
{
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

void Scene::AddObject(GameObject gameObject)
{
	if (find(objects->begin(), objects->end(), gameObject) != objects->end()) return;

	objects->push_back(gameObject);
	std::sort(objects->begin(), objects->end(), Scene::Comparator);
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
