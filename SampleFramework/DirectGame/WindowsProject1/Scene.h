#pragma once
#include <string>
#include <windows.h>
#include <queue>
#include "GameObject.h"
#include "KeyEventHandler.h"
#include "Camera.h"

class Scene
{
public:
	Scene();
	Scene(std::string filePath);
	~Scene();
	virtual void Load();
	virtual void Unload();
	virtual void Update();
	virtual void Render();
	void CleanDestroyedObjects();

	std::string GetID() { return id; }
	void AddObject(GameObject gameObject);
	void RemoveObject(GameObject gameObject);

	std::vector<GameObject>* GetSceneObjects();
	void SetMainCamera(Camera* camera);
	Camera* GetMainCamera();

protected:
	static bool Comparator(GameObject& a, GameObject& b) 
	{ 
		return (a->GetRenderOrder() < b->GetRenderOrder() || 
			(a->GetRenderOrder() == b->GetRenderOrder() && a->GetTransform().Position.x < b->GetTransform().Position.x)); 
	}
	
	std::vector<GameObject>* objects;
	std::vector<GameObject> destroyed;
	std::string id;
	std::string filePath;
	Camera* mainCamera;

	bool loaded;
};

