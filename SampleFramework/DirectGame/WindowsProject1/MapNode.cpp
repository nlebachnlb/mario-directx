#include "MapNode.h"

void MapNode::Awake()
{
	rigidbody->SetDynamic(false);

	InitAnimations();
}

void MapNode::LinkData(GraphNode* node)
{
	linkedDataNode = node;
}

GraphNode* MapNode::GetData()
{
	return linkedDataNode;
}
