#include "MapNode.h"

void MapNode::Awake()
{
	rigidbody->SetDynamic(false);

	InitAnimations();
}
