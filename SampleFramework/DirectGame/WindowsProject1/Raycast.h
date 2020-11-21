#pragma once
#include "GameObject.h"
#include <vector>

enum class HRayDirection
{
	Left, Right
};

class Raycast
{
public:
	Raycast();
	~Raycast();

	void SetCastObjects(std::vector<GameObject>* obj);
	void HShoot(Vector2 startPoint, HRayDirection direction, ObjectTags tag, float rayDistance, std::vector<Collider2D*>& result);

private:
	std::vector<GameObject>* objects;
};

