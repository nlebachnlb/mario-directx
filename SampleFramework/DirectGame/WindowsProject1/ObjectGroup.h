#pragma once
#include <string>
#include <vector>
#include "tinyxml.h"

struct Object
{
	int id;
	int x, y, width, height;
};

class ObjectGroup
{
public:
	ObjectGroup();
	~ObjectGroup();
	
	static ObjectGroup* FromXMLData(TiXmlElement* data);

	std::string GetName();
	int GetID();

	std::vector<Object*>* GetObjects();
private:
	int id;
	std::string name;
	std::vector<Object*>* objects;
};

