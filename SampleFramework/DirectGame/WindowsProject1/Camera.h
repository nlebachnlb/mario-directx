#pragma once
#include "Transform.h"
#include "GameMap.h"
#include "GameObject.h"

enum class ScrollMode
{
	Targeting,
	Automatic
};

struct BoundarySet
{
	Vector2 position;
	RectF boundary;
	ScrollMode mode;
	std::vector<Vector2> path;
	float pathSpeed;

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

	bool RectInsideCameraView(RectF rect, int boundThickness = 0);
	bool PointInsideCameraView(Vector2 point, int boundWidth);

	Vector2 GetPosition();
	void SetPosition(Vector2 newPos);

	Vector2 GetViewportSize();

	Vector2 WorldToViewport(Vector2 srcPosition);
	Vector2 ViewportToWorld(Vector2 srcPosition);

	void SetTarget(GameObject gameObject);
	void SetBoundary(RectF boundary);
	RectF GetCurrentBoundary();
	void SetRenderOffset(Vector2 offset);

	void AddBoundarySet(int id, Vector2 position, RectF boundary);
	void AddBoundarySet(int id, BoundarySet bSet);
	void SetCurrentBoundarySet(int id);
	BoundarySet GetBoundarySet(int id);

	void FreeBoundary();
	void LockBoundary();

	void LockCamera();
	void UnlockCamera();

	void Shake(int timeout);

	void SetScrollMode(ScrollMode mode);
	ScrollMode GetScrollMode();

	RectF GetBoundingBox();

protected:
	Vector2 position, viewportSize, targetPivot;
	GameMap* map;
	std::unordered_map<int, BoundarySet> boundaries;
	int bottomOffset;
	int boundaryLocked;
	float followSpeed;
	ScrollMode scrollMode;
	int currentBoundarySet;

private:
	void Initialize();
	void TargetingMode();
	void AutoscrollingMode();

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
	RectF lastBoundary;
	Vector2 renderOffset;
	int shaking, shakingTimer;

	float timer; 
	int currentPathNode;
	Vector2 startPosition;
};

