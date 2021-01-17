#pragma once
#include "GameObject.h"
#include <unordered_set>

struct Index { int x, y; };

class CGameObject;
typedef CGameObject* GameObject;

class Grid;
class Cell
{
public:
	Cell(int x, int y);
	~Cell();

	void Insert(GameObject object);
	void Remove(GameObject object);
	std::unordered_set<GameObject>* GetObjects();

	Index GetIndex();
	Grid* GetContainingGrid();

private:
	std::unordered_set<GameObject>* objects;
	Index index;
	Grid* grid;
	friend class Grid;
};

