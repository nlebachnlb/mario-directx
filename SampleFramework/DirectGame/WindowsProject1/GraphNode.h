#pragma once
#include <vector>
#include "Transform.h"

enum class Weight
{
	None, Left, Right, Up, Down
};

struct Edge
{
	int nodeID;
	Weight weight;
};

class GraphNode
{
public:
	GraphNode(int id);
	~GraphNode();

	int GetNodeID();
	void SetPosition(Vector2 position);
	Vector2 GetPosition();

	std::vector<Edge>* GetAdjacentList();
private:
	int nodeID;
	Vector2 position;
	std::vector<Edge>* adjacentNodes;
};

