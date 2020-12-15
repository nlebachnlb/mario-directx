#include "Graph.h"

void Graph::InsertNode(GraphNode* node)
{
	nodes[node->GetNodeID()] = node;
}

GraphNode* Graph::GetNode(int id)
{
	return nodes[id];
}
