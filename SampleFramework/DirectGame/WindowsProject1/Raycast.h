#pragma once
#include "GameObject.h"
#include <vector>

enum class HRayDirection
{
	Left, Right
};

enum class VRayDirection
{
	Up, Down
};

class Raycast
{
public:
	Raycast();
	~Raycast();

	void SetCastObjects(std::vector<GameObject>* obj);
	void HShoot(Vector2 startPoint, HRayDirection direction, ObjectTags tag, float rayDistance, std::vector<Collider2D*>& result);
	bool VerticalHit(Vector2 startPoint, VRayDirection direction, ObjectTags tag, float rayDistance);

private:
	std::vector<GameObject>* objects;
};

