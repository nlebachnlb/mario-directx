#include "GraphNode.h"

GraphNode::GraphNode()
{
    adjacentNodes = new std::vector<GraphNode*>();
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

Weight GraphNode::GetWeight()
{
    return weight;
}

std::vector<GraphNode*>* GraphNode::GetAdjacentList()
{
    return adjacentNodes;
}
