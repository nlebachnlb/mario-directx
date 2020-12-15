#pragma once
#include "GraphNode.h"
#include <unordered_map>

class Graph
{
public:
	void InsertNode(GraphNode* node);
	GraphNode* GetNode(int id);
private:
	std::unordered_map<int, GraphNode*> nodes;
};

