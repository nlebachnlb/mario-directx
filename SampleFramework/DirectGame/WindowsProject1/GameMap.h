#pragma once
#ifndef __GAMEMAP_H__
#define __GAMEMAP_H__
#include "Service.h"
#include "Tile.h"
#include <unordered_map>
#include "GameObject.h"
#include "MapData.h"
#include "SpawnerManager.h"

class GameMap : public Service
{
public:
	GameMap();
	GameMap(std::string filePath);
	void Load(std::string filePath = "", bool manual = true);

	void Initialization() override;
	MapData* GetMapData();
	std::vector<GameObject> GetGameObjects();

	Tile GetTileset(int id);
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
