#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "tinyxml.h"

struct Object
{
	int id;
	int x, y, width, height;
	std::string name, type;
	std::unordered_map<std::string, std::string> properties;

	std::string GetPropertyValue(std::string propertyName);

	int cellx, celly;
};

class ObjectGroup
{
public:
	ObjectGroup();
	~ObjectGroup();
	
	static ObjectGroup* FromXMLData(TiXmlElement* data);
	void IntegrateGridData(TiXmlElement* data);

	std::string GetName();
	int GetID();

	std::vector<Object*>* GetObjects();
private:
	int id;
	std::string name;
	std::vector<Object*>* objects;
	std::unordered_map<int, Object*>* objectDictionary;
};

