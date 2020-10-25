#pragma once
#include "Transform.h"
#include "GameMap.h"
#include "GameObject.h"

struct BoundarySet
{
	Vector2 position;
	RectF boundary;

	static BoundarySet Empty()
	{
		return BoundarySet{ Vector2(-1, -1), RectF{ -1, -1, -1, -1 } };
	}
};

class Camera
{
public:
	Camera();
	Camera(Vector2 startPosition, Vector2 viewportSize);
	~Camera();

	virtual void Update();
	virtual void Render();

	bool RectInsideCameraView(RECT rect);

	Vector2 GetPosition();
	void SetPosition(Vector2 newPos);

	Vector2 GetViewportSize();

	Vector2 WorldToViewport(Vector2 srcPosition);
	Vector2 ViewportToWorld(Vector2 srcPosition);

	void SetTarget(GameObject gameObject);
	void SetBoundary(RectF boundary);

	void AddBoundarySet(int id, Vector2 position, RectF boundary);
	void AddBoundarySet(int id, BoundarySet bSet);
	BoundarySet GetBoundarySet(int id);

protected:
	Vector2 position, viewportSize;
	GameMap* map;
	std::unordered_map<int, BoundarySet> boundaries;

private:
	GameObject target;
	RectF boundary;
};

