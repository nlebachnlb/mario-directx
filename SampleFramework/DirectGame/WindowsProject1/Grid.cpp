#include "Grid.h"
#include "Mathf.h"

Grid::Grid(GridConfig config)
{
	this->config = config;
	widthInCells = Mathf::Ceil((float)config.width / (float)config.cellWidth);
	heightInCells = Mathf::Ceil((float)config.height / (float)config.cellHeight);

	for (int x = 0; x < widthInCells; ++x)
	{
		std::vector<Cell*> column;
		for (int y = 0; y < heightInCells; ++y)
		{
			auto cell = new Cell(x, y);
			cell->grid = this;
			column.push_back(cell);
		}
		cells.push_back(column);
	}
}

Grid::~Grid()
{
	for (int x = 0; x < widthInCells; ++x)
	{
		for (int y = 0; y < heightInCells; ++y)
			delete cells.at(x).at(y);
		cells.at(x).clear();
	}
	cells.clear();
}

GridConfig Grid::GetConfig()
{
	return config;
}

void Grid::UpdateObject(GameObject object)
{
	//auto oldPosition = object->GetPreviousTransform().Position;
	auto newPosition = object->GetTransform().Position;

	int x = (int)(newPosition.x / config.cellWidth);
	int y = (int)(newPosition.y / config.cellHeight);

	Cell* newCell = GetCell(x, y);

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

Cell* Grid::GetCell(int x, int y)
{
	x = Mathf::ClampInt(x, 0, widthInCells - 1);
	y = Mathf::ClampInt(y, 0, heightInCells - 1);
	return cells.at(x).at(y);
}

Index Grid::ToIndex(Vector2 position)
{
	return Index({
		(int)(position.x / config.cellWidth),
		(int)(position.y / config.cellHeight)
	});
}

void Grid::Insert(GameObject object)
{
	int x = (int)(object->GetTransform().Position.x / config.cellWidth);
	int y = (int)(object->GetTransform().Position.y / config.cellHeight);
	Insert(object, x, y);
}

void Grid::Remove(GameObject object)
{
	int x = (int)(object->GetTransform().Position.x / config.cellWidth);
	int y = (int)(object->GetTransform().Position.y / config.cellHeight);
	Cell* cell = GetCell(x, y);
	cell->Remove(object);
}

void Grid::Insert(GameObject object, Index cellIndex)
{
	Cell* cell = GetCell(cellIndex);
	object->SetCell(cell);
	cell->Insert(object);
}

void Grid::Insert(GameObject object, int x, int y)
{
	Cell* cell = GetCell(x, y);
	object->SetCell(cell);
	cell->Insert(object);
}

void Grid::GetActiveCells(RectF rect, std::vector<Cell*>& result)
{
	int startx = (int)(Mathf::Floor(rect.left / (float)config.cellWidth));
	int starty = (int)(Mathf::Floor(rect.top / (float)config.cellHeight));
	int endx = (int)(Mathf::Ceil(rect.right / (float)config.cellWidth));
	int endy = (int)(Mathf::Ceil(rect.bottom / (float)config.cellHeight));
	result.clear();

	for (int x = startx; x <= endx; ++x)
	{
		if (x < 0 || x >= widthInCells) continue;
		for (int y = starty; y <= endy; ++y)
		{
			if (y < 0 || y >= heightInCells) continue;
			Cell* cell = GetCell(x, y);
			result.push_back(cell);
		}
	}
}
