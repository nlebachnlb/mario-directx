#pragma once
#include <unordered_set>
#include "ObjectTags.h"

struct PhysicMaterial
{
	float bounciness;
	float friction;
	std::unordered_set<ObjectTags> bypass;

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
		bypass.insert(tag);
	}
};

