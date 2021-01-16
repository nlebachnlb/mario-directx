#pragma once
#include <vector>
#include "Cell.h"

struct GridConfig
{
	int cellSize;
	int width, height;
};

class Grid
{
public:
	Grid(GridConfig config);
	~Grid();

	GridConfig GetConfig();

	void UpdateObject(GameObject object);
	Cell* GetCell(Index index);
	Index ToIndex(Vector2 position);

	void Insert(GameObject object);
	void Remove(GameObject object);
	void Insert(GameObject object, Index cellIndex);

	void GetActiveCells(RectF rect, std::vector<Cell*>& result);

private:
	std::vector<std::vector<Cell*>> cells;
	GridConfig config;
	int widthInCells, heightInCells;
};

