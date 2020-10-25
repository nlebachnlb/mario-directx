#pragma once
#include <string>
#include <map>
#include "tinyxml.h"
#include "TilesetInfo.h"
#include "Layer.h"
#include "ObjectGroup.h"

class MapData
{
public:
	MapData();
	~MapData();

	static MapData* FromTMX(std::string path);

	int GetTilesetIdFromTileId(int tileId);

	int GetMapWidthInPixels();
	int GetMapHeightInPixels();

	int GetMapWidthInTiles();
	int GetMapHeightInTiles();

	int GetTileWidth();
	int GetTileHeight();
	std::map<int, TilesetInfo>* GetTilesets();
	std::map<int, Layer*>* GetLayers();
	std::map<int, ObjectGroup*>* GetObjectGroups();
private:
	int width, height;
	int tileWidth, tileHeight;
	std::map<int, TilesetInfo>* tilesets;
	std::map<int, Layer*>* layers;
	std::map<int, ObjectGroup*>* objectGroups;
};

