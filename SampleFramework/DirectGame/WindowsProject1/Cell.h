#pragma once
#include "GameObject.h"
#include <unordered_set>

struct Index { int x, y; };

class Cell
{
public:
	Cell(int x, int y);
	~Cell();

	void Insert(GameObject object);
	void Remove(GameObject object);

	Index GetIndex();

private:
	std::unordered_set<GameObject>* objects;
	Index index;
};

