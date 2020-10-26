#pragma once
#include <vector>
#include "GameObject.h"
#include "Scene.h"

class ObjectPool
{
public:
	void Add(GameObject object);
	GameObject Instantiate();
	void Revoke(GameObject obj);
	bool IsEmpty();
	void RegisterPoolToScene(Scene* scene);
private:
	std::vector<GameObject> pool;
};