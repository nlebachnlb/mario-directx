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
	for (int x = 0; x < widthInCells; ++x)
		for (int y = 0; y < heightInCells; ++y)
			delete cells.at(x).at(y);
}

GridConfig Grid::GetConfig()
{
	return config;
}

void Grid::UpdateObject(GameObject object)
{
	//auto oldPosition = object->GetPreviousTransform().Position;
	auto newPosition = object->GetTransform().Position;
	//Index oldIndex = Index({ 
	//	(int)(oldPosition.x / config.cellSize),
	//	(int)(oldPosition.y / config.cellSize)
	//});
	Index newIndex = Index({
		(int)(newPosition.x / config.cellSize),
		(int)(newPosition.y / config.cellSize)
	});

	//if (oldIndex.x == newIndex.x && oldIndex.y == newIndex.y) return;

	//Cell* oldCell = GetCell(oldIndex);
	Cell* newCell = GetCell(newIndex);

	//oldCell->Remove(object);
	object->GetCell()->Remove(object);
	newCell->Insert(object);
	object->SetCell(newCell);
}

Cell* Grid::GetCell(Index index)
{
	int x = Mathf::ClampInt(index.x, 0, widthInCells - 1); 
	int y = Mathf::ClampInt(index.y, 0, heightInCells - 1);
	return cells.at(x).at(y);
}

Index Grid::ToIndex(Vector2 position)
{
	return Index({
		(int)(position.x / config.cellSize),
		(int)(position.y / config.cellSize)
	});
}

void Grid::Insert(GameObject object)
{
	Index autoIndex = Index({
		(int)(object->GetTransform().Position.x / config.cellSize),
		(int)(object->GetTransform().Position.y / config.cellSize)
	});
	Insert(object, autoIndex);
}

void Grid::Remove(GameObject object)
{
	Index autoIndex = ToIndex(object->GetTransform().Position);
	Cell* cell = GetCell(autoIndex);
	cell->Remove(object);
}

void Grid::Insert(GameObject object, Index cellIndex)
{
	Cell* cell = GetCell(cellIndex);
	object->SetCell(cell);
	cell->Insert(object);
}

void Grid::GetActiveCells(RectF rect, std::vector<Cell*>& result)
{
	Index start = ToIndex(Vector2(rect.left, rect.top));
	Index end = ToIndex(Vector2(rect.right, rect.bottom));
	result.clear();

	for (int x = start.x; x <= end.x; ++x)
	{
		for (int y = start.y; y <= end.y; ++y)
		{
			Cell* cell = GetCell(Index({ x, y }));
			result.push_back(cell);
		}
	}
}
