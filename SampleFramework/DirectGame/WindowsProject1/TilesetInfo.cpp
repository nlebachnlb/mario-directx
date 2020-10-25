#include "TilesetInfo.h"

TilesetInfo::TilesetInfo()
{
}

TilesetInfo::TilesetInfo(int id, int tileCount, std::string source)
{
	this->id = id;
	this->tileCount = tileCount;
	this->source = source;
}

TilesetInfo* TilesetInfo::FromXMLData(TiXmlElement* data)
{
	TilesetInfo* tileset = new TilesetInfo();
	
	data->QueryIntAttribute("firstgid", &tileset->id);
	data->QueryIntAttribute("tilecount", &tileset->tileCount);
	
	auto image = data->FirstChildElement("image");
	tileset->source = image->Attribute("source");
	image->QueryIntAttribute("width", &tileset->imgWidth);
	image->QueryIntAttribute("height", &tileset->imgHeight);

	return tileset;
}

std::string TilesetInfo::GetSource()
{
	return this->source;
}

int TilesetInfo::GetID()
{
	return this->id;
}

int TilesetInfo::GetTileCount()
{
	return this->tileCount;
}

int TilesetInfo::GetImageWidth()
{
	return this->imgWidth;
}

int TilesetInfo::GetImageHeight()
{
	return this->imgHeight;
}
