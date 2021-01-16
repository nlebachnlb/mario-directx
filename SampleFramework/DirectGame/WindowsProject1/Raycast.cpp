#include "Raycast.h"
#include <algorithm>
#include "Mathf.h"

Raycast::Raycast()
{
	objects = nullptr;
}

Raycast::~Raycast()
{
}

void Raycast::SetCastObjects(std::vector<GameObject>* obj)
{
	this->objects = obj;
}

void Raycast::HShoot(Vector2 startPoint, HRayDirection direction, ObjectTags tag, float rayDistance, std::vector<Collider2D*>& result)
{
	std::vector<GameObject> considered;
	result.clear();
	for (auto o : *objects)
	{
		if (o->IsDestroyed()) continue;
		if (!o->IsEnabled()) continue;
		if (o->GetTag() != tag) continue;
		if (o->GetColliders()->size() == 0) continue;

		if (direction == HRayDirection::Left && 
			Mathf::InRange(o->GetTransform().Position.x, startPoint.x - rayDistance, startPoint.x))
			considered.push_back(o);
		else if (direction == HRayDirection::Right && 
			Mathf::InRange(o->GetTransform().Position.x, startPoint.x, startPoint.x + rayDistance))
			considered.push_back(o);
	}

	std::sort(considered.begin(), considered.end(), [](GameObject& a, GameObject& b)
		{
			return a->GetTransform().Position.x < b->GetTransform().Position.x;
		}
	);

	// If ray faces to left, the x coordinates should be in descending order
	if (direction == HRayDirection::Left) std::reverse(considered.begin(), considered.end());

	auto y0 = startPoint.y;
	for (auto o : considered)
	{
		auto bbox = o->GetColliders()->at(0)->GetBoundingBox();
		bool yCondition = Mathf::InRange(y0, bbox.top, bbox.bottom);
		bool xCondition;
		if (direction == HRayDirection::Left)
			xCondition = bbox.right <= startPoint.x;
		else if (direction == HRayDirection::Right)
			xCondition = startPoint.x <= bbox.left;

		if (xCondition && yCondition) result.push_back(o->GetColliders()->at(0));
	}
}

bool Raycast::VerticalHit(Vector2 startPoint, VRayDirection direction, ObjectTags tag, float rayDistance)
{
	for (auto o : *objects)
	{
		if (o->IsDestroyed()) continue;
		if (!o->IsEnabled()) continue;
		if (o->GetTag() != tag) continue;
		if (o->GetColliders()->size() == 0) continue;

		auto box = o->GetColliders()->at(0)->GetBoundingBox();
		if (direction == VRayDirection::Down &&
			box.top < startPoint.y + rayDistance && box.bottom > startPoint.y)
			return true;
		else if (direction == VRayDirection::Up &&
			box.bottom > startPoint.y - rayDistance && box.top < startPoint.y)
			return true;
	}

	return false;
}