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

std::vector<Edge>* GraphNode::GetAdjacentList()
{
    return adjacentNodes;
}
