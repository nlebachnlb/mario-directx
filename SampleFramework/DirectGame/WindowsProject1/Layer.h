#pragma once
#include <string>
#include <vector>
#include "tinyxml.h"

class Layer
{
public:
	Layer();
	~Layer();

	static Layer* FromXMLData(TiXmlElement* data);

	int GetID();
	std::string GetName();

	int GetWidth();
	int GetHeight();

	int GetTileID(int x, int y);

private:
	int id;
	std::string name;
	int width, height;
	std::vector<std::vector<int>*>* data;
};

