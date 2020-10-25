#pragma once
#include <string>
#include "tinyxml.h"

class TilesetInfo
{
public:
	TilesetInfo();
	TilesetInfo(int id, int tileCount, std::string source);

	static TilesetInfo* FromXMLData(TiXmlElement* data);

	std::string GetSource();
	int GetID();
	int GetTileCount();
	int GetImageWidth();
	int GetImageHeight();

private:
	int id;
	std::string source;
	int tileCount;
	int imgWidth, imgHeight;
};

