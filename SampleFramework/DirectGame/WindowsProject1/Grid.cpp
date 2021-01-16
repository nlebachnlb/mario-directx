#include "Grid.h"
#include "Mathf.h"

Grid::Grid(GridConfig config)
{
	this->config = config;
	widthInCells = Mathf::Ceil((float)config.width / (float)config.cellSize);
	heightInCells = Mathf::Ceil((float)config.height / (float)config.cellSize);

	for (int x = 0; x < widthInCells; ++x)
	{
		std::vector<Cell*> column;
		for (int y = 0; y < heightInCells; ++y)
		{
			auto cell = new Cell(x, y);
			column.push_back(cell);
		}
		cells.push_back(column);
	}
}

Grid::~Grid()
{
}

GridConfig Grid::GetConfig()
{
	return config;
}

void Grid::UpdateObject(GameObject object)
{
	auto oldPosition = object->GetPreviousTransform().Position;
	auto newPosition = object->GetTransform().Position;
	Index oldIndex = Index({ 
		(int)(oldPosition.x / config.cellSize),
		(int)(oldPosition.y / config.cellSize)
	});
	Index newIndex = Index({
		(int)(newPosition.x / config.cellSize),
		(int)(newPosition.y / config.cellSize)
	});

	if (oldIndex.x == newIndex.x && oldIndex.y == newIndex.y) return;

	Cell* oldCell = GetCell(oldIndex);
	Cell* newCell = GetCell(newIndex);

	oldCell->Remove(object);
	newCell->Insert(object);
}

Cell* Grid::GetCell(Index index)
{
	int x = Mathf::Clamp(index.x, 0, widthInCells - 1); 
	int y = Mathf::Clamp(index.y, 0, heightInCells - 1);
	return cells.at(x).at(y);
}

void Grid::Insert(GameObject object)
{
	Index autoIndex = Index({
		(int)(object->GetTransform().Position.x / config.cellSize),
		(int)(object->GetTransform().Position.y / config.cellSize)
	});
	Insert(object, autoIndex);
}

void Grid::Insert(GameObject object, Index cellIndex)
{
	Cell* cell = GetCell(cellIndex);
	cell->Insert(object);
}
