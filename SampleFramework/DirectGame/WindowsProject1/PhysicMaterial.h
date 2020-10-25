#pragma once
struct PhysicMaterial
{
	float bounciness;
	float friction;

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
};

