#pragma once
#include <vector>
#include "ObjectTags.h"

struct PhysicMaterial
{
	float bounciness;
	float friction;
	std::vector<ObjectTags> bypass;

	static PhysicMaterial Empty()
	{
		return PhysicMaterial(0, 1);
	}

	PhysicMaterial()
	{
		(*this) = Empty();
	}

	PhysicMaterial(float bounciness, float friction)
	{
		this->bounciness = bounciness;
		this->friction = friction;
	}

	void AddBypassTag(ObjectTags tag)
	{
		bypass.push_back(tag);
	}
};

