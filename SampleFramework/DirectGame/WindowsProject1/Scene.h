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
	virtual void Init();
	virtual void Unload();
	virtual void Update();
	virtual void Render();
	void CleanDestroyedObjects();
	void ProcessInstantiateRequests();

	std::string GetID() { return id; }
	void AddObject(GameObject gameObject);
	void RemoveObject(GameObject gameObject);

	std::vector<GameObject>* GetSceneObjects();
	std::vector<GameObject>& GetActiveObjects();

	void SetMainCamera(Camera* camera);
	Camera* GetMainCamera();

protected:
	void Remove(GameObject go);
	static bool Comparator(const GameObject& a, const GameObject& b) 
	{ 
		return (a->GetRenderOrder() < b->GetRenderOrder() || 
			(a->GetRenderOrder() == b->GetRenderOrder() && a->GetTransform().Position.x < b->GetTransform().Position.x)); 
	}
	
	std::vector<GameObject>* objects;
	std::vector<GameObject> destroyed, updated, instantiated;
	std::string id;
	std::string filePath;
	Camera* mainCamera;

	bool loaded;
};

