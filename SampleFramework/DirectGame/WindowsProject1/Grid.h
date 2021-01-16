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

	void Insert(GameObject object);
	void Insert(GameObject object, Index cellIndex);

private:
	std::vector<std::vector<Cell*>> cells;
	GridConfig config;
	int widthInCells, heightInCells;
};

