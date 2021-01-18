#include "Cell.h"

Cell::Cell(int x, int y)
{
	index.x = x;
	index.y = y;
	objects = new std::unordered_set<GameObject>();
}

Cell::~Cell()
{
	objects->clear();
	delete objects;
}

void Cell::Insert(GameObject object)
{
	if (objects->find(object) == objects->end())
		objects->insert(object);
}

void Cell::Remove(GameObject object)
{
	if (objects->find(object) != objects->end())
		objects->erase(object);
}

std::unordered_set<GameObject>* Cell::GetObjects()
{
	return objects;
}

Index Cell::GetIndex()
{
	return index;
}

Grid* Cell::GetContainingGrid()
{
	return grid;
}
