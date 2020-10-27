#pragma once
#include <vector>
#include "ObjectTags.h"
#include "Transform.h"

struct PhysicMaterial
{
	Vector2 bounciness;
	float friction;
	std::vector<ObjectTags> bypass;

	static PhysicMaterial Empty()
	{
		return PhysicMaterial(VectorZero(), 1);
	}

	PhysicMaterial()
	{
		(*this) = Empty();
	}

	PhysicMaterial(Vector2 bounciness, float friction)
	{
		this->bounciness = bounciness;
		this->friction = friction;
	}

	void AddBypassTag(ObjectTags tag)
	{
		bypass.push_back(tag);
	}
};

