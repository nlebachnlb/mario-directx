#include "GraphNode.h"

GraphNode::GraphNode(int id)
{
    this->nodeID = id;
    adjacentNodes = new std::vector<Edge>();
}

GraphNode::~GraphNode()
{
    adjacentNodes->clear();
    delete adjacentNodes;
}

int GraphNode::GetNodeID()
{
    return nodeID;
}

void GraphNode::SetPosition(Vector2 position)
{
    this->position = position;
}

Vector2 GraphNode::GetPosition()
{
    return position;
}

std::vector<Edge>* GraphNode::GetAdjacentList()
{
    return adjacentNodes;
}
