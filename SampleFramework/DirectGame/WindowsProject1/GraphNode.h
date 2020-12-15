#pragma once
#include <vector>

enum class Weight
{
	None, Left, Right, Up, Down
};

class GraphNode
{
public:
	GraphNode();
	~GraphNode();

	int GetNodeID();
	Weight GetWeight();
	std::vector<GraphNode*>* GetAdjacentList();
private:
	int nodeID;
	Weight weight;
	std::vector<GraphNode*>* adjacentNodes;
};

