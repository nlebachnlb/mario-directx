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
	void LoadMapData(std::string filePath = "", std::string metaPath = "");
	void Load();
	void Unload();

	void LoadEnvironment();
	void LoadEnemy();

	void Initialization() override;
	MapData* GetMapData();
	std::vector<GameObject> GetGameObjects();
	std::vector<int> GetIDs();

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
	std::vector<Texture2D> textures;
	std::vector<GameObject> gameObjects;
	std::vector<int> objectIDs;
	SpawnerManager* spawnerManager;
};
#endif
