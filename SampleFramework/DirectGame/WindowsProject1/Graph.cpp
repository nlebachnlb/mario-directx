#include "Graph.h"

void Graph::InsertNode(GraphNode* node)
{
	nodes[node->GetNodeID()] = node;
}

GraphNode* Graph::GetNode(int id)
{
	if (nodes.find(id) == nodes.end()) return nullptr;
	return nodes[id];
}
