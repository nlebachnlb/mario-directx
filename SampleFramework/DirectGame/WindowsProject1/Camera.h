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
	virtual void Render(std::vector<GameObject>& objs);

	bool RectInsideCameraView(RectF rect);
	bool PointInsideCameraView(Vector2 point, int boundWidth);

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
	Vector2 position, viewportSize, targetPivot;
	GameMap* map;
	std::unordered_map<int, BoundarySet> boundaries;

private:
	void Initialize();
	GameObject target;
	RectF boundary;
	// Render temp variables
	MapData* mapData;
	std::map<int, Layer*>* layers;
	std::map<int, TilesetInfo*>* tilesets;
	int tileWidth, tileHeight;
	int width, height; // Number of tiles on viewport
	int mapWidth, mapHeight;
	bool initialized;
};

