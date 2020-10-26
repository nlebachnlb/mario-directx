#pragma once
#include <queue>
#include "GameObject.h"

class ObjectPool
{
public:
	void Add(GameObject object);
	GameObject Instantiate();
	void Revoke(GameObject obj);
	bool IsEmpty();
private:
	std::queue<GameObject> pool;
};