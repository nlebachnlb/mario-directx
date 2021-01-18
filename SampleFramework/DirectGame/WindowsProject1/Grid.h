#pragma once
#include <vector>
#include "Cell.h"

struct GridConfig
{
	int cellWidth, cellHeight;
	int width, height;
};

class Cell;
class Grid
{
public:
	Grid(GridConfig config);
	~Grid();

	GridConfig GetConfig();

	void UpdateObject(GameObject object);
	Cell* GetCell(Index index);
	Cell* GetCell(int x, int y);
	Index ToIndex(Vector2 position);

	void Insert(GameObject object);
	void Remove(GameObject object);
	void Insert(GameObject object, Index cellIndex);
	void Insert(GameObject object, int x, int y);

	void GetActiveCells(RectF rect, std::vector<Cell*>& result);

private:
	std::vector<std::vector<Cell*>> cells;
	GridConfig config;
	int widthInCells, heightInCells;
};

