#pragma once
#ifndef __GAMEMAP_H__
#define __GAMEMAP_H__
#include "Service.h"
#include "Tile.h"
#include <unordered_map>
#include "GameObject.h"
#include "MapData.h"
#include "SpawnerManager.h"

const int PIPE_TILESET_ID = 100;
const int PIPE_START_X = 0;
const int PIPE_START_Y = 0;
const int PIPE_HEIGHT = 192;
const int PIPE_WIDTH = 672;

class GameMap : public Service
{
public:
	GameMap();
	GameMap(std::string filePath);
	void Load(std::string filePath = "", bool manual = true);
	void LoadEnemy();

	void Initialization() override;
	MapData* GetMapData();
	std::vector<GameObject> GetGameObjects();

	Tile GetTileset(int id);
	Tile GetPipeTileset();
	int GetMapWidth();
	int GetMapHeight();

	void Update();
	void Render();

	SpawnerManager* GetSpawnerManager();

	~GameMap();

private:
	std::string filePath;
	MapData* mapData;
	std::unordered_map<int, Tile> tilesets;
	std::vector<GameObject> gameObjects;
	SpawnerManager* spawnerManager;
};
#endif
