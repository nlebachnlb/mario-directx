#pragma once
#include <string>
#include <vector>
#include "tinyxml.h"

class Layer
{
public:
	Layer();
	~Layer();

	static Layer* FromXMLData(TiXmlElement* data, int index = -1);

	int GetID();
	std::string GetName();

	int GetWidth();
	int GetHeight();

	int GetTileID(int x, int y);

	bool IsVisible();

private:
	int id;
	std::string name;
	int width, height;
	std::vector<std::vector<int>*>* data;
	bool visible;
};

