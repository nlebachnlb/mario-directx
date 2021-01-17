#pragma once
#include <string>
#include <windows.h>
#include <queue>
#include "GameObject.h"
#include "KeyEventHandler.h"
#include "Camera.h"
#include "Grid.h"

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
	void UpdateActiveObjects();

	void SetFilePath(std::string path);
	std::string GetID() { return id; }
	void AddObject(GameObject gameObject);
	void RemoveObject(GameObject gameObject);

	std::vector<GameObject>* GetSceneObjects();
	std::vector<GameObject>& GetActiveObjects();

	void SetMainCamera(Camera* camera);
	Camera* GetMainCamera();

	Grid* GetGrid();

protected:
	void Remove(GameObject go);
	static bool Comparator(const GameObject& a, const GameObject& b) 
	{ 
		auto apos = a->GetTransform().Position;
		auto bpos = b->GetTransform().Position;
		return (a->GetRenderOrder() < b->GetRenderOrder() || 
			(a->GetRenderOrder() == b->GetRenderOrder() && 
			(apos.x < bpos.x || 
				(apos.x == bpos.x &&
					apos.y < bpos.y)))
		); 
	}
	
	std::vector<GameObject>* objects;
	std::vector<GameObject> destroyed, updated, instantiated, inCells, globalObjects;
	std::vector<Cell*> activeCells;
	std::string id;
	std::string filePath;
	Camera* mainCamera;
	Grid* grid;
	bool needSpatialPartition;

	bool loaded;
};

