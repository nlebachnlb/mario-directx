#include "Camera.h"
#include "Consts.h"
#include "Game.h"
#include <cmath>
#include "Mathf.h"

Camera::Camera()
{
    auto configs = Game::GetInstance().GetGlobalConfigs();
	this->viewportSize = Vector2(configs.screenWidth, configs.screenHeight);
	this->position = VectorZero();
    this->map = nullptr;
    this->targetPivot = Vector2(0.5f, 0.65f);
    this->bottomOffset = configs.hudOffset;
    this->followSpeed = 30.0f;
    initialized = false;
    lastBoundary = RectF::Empty();
    renderOffset = VectorZero();
    scrollMode = ScrollMode::Targeting;
}

Camera::Camera(Vector2 startPosition, Vector2 viewportSize)
{
	this->position = startPosition;
	this->viewportSize = viewportSize;
    initialized = false;
}

Camera::~Camera()
{

}

void Camera::Update()
{
    if (map == nullptr) map = Game::GetInstance().GetService<GameMap>();
    switch (scrollMode)
    {
    case ScrollMode::Targeting: TargetingMode(); break;
    case ScrollMode::Automatic: AutoscrollingMode(); break;
    }
}

void Camera::Render(std::vector<GameObject>& objs)
{
    if (!initialized) Initialize();
    if (mapData != nullptr)
    {
        Vector2 translation = -1 * GetPosition();
        auto camPos = GetPosition();
        int tilex = (int)(camPos.x / tileWidth);
        int tiley = (int)(camPos.y / tileHeight);

        for (auto l_data : *layers)
        {
            auto layer = l_data.second;
            if (layer->IsVisible() == false) continue;

            for (int u = 0; u <= height - 4 + 2; ++u)
            {
                int yGrid = (int)(tiley + u);
                if (yGrid >= mapHeight) continue;

                for (int v = 0; v <= width + 1; ++v)
                {
                    int xGrid = (int)(tilex + v);
                    if (xGrid >= mapWidth) continue;

                    int tileId = layer->GetTileID(xGrid, yGrid);
                    int tilesetId = mapData->GetTilesetIdFromTileId(tileId);

                    if (tilesetId == -1) continue;

                    auto tileset = tilesets->at(tilesetId);

                    int tilesetWidth = tileset->GetImageWidth() / tileWidth;
                    int tilesetHeight = tileset->GetImageHeight() / tileHeight;

                    int temp = tileId - tilesetId;
                    int y = temp / tilesetWidth;
                    int x = temp - y * tilesetWidth;

                    Tile tile = map->GetTileset(tilesetId);
                    tile->SetSourceRect(x * tileWidth, y * tileHeight, tileWidth, tileHeight);

                    Vector2 finalPosition(xGrid * tileWidth, yGrid * tileHeight);
                    finalPosition = finalPosition + translation + renderOffset;
                    tile->Draw(finalPosition.x, finalPosition.y);
                }
            }
        }
    }

    for (auto o : objs)
        if (o->IsEnabled()) o->Render(-(GetPosition() - renderOffset));
}

bool Camera::RectInsideCameraView(RectF rect)
{
	auto rectWidth = rect.right - rect.left;
	auto rectHeight = rect.bottom - rect.top;

	return rect.left >= position.x - rectWidth && rect.right <= position.x + viewportSize.x + rectWidth &&
			rect.top >= position.y - rectHeight && rect.bottom <= position.y + viewportSize.y + rectHeight;
}

bool Camera::PointInsideCameraView(Vector2 point, int boundThickness)
{
    return point.x >= position.x - boundThickness && point.x <= position.x + viewportSize.x + boundThickness && 
            point.y >= position.y - boundThickness && point.y <= position.y + viewportSize.y + boundThickness;
}

Vector2 Camera::GetPosition()
{
	return position;
}

void Camera::SetPosition(Vector2 newPos)
{
	position = newPos;
}

Vector2 Camera::GetViewportSize()
{
	return viewportSize;
}

Vector2 Camera::WorldToViewport(Vector2 srcPosition)
{
	return srcPosition - position;
}

Vector2 Camera::ViewportToWorld(Vector2 srcPosition)
{
	return srcPosition + position;
}

void Camera::SetTarget(GameObject gameObject)
{
    this->target = gameObject;
}

void Camera::SetBoundary(RectF boundary)
{
    lastBoundary = boundary;
    this->boundary = boundary;
}

RectF Camera::GetCurrentBoundary()
{
    return boundary;
}

void Camera::SetRenderOffset(Vector2 offset)
{
    renderOffset = offset;
}

void Camera::AddBoundarySet(int id, Vector2 position, RectF boundary)
{
    boundaries.insert(make_pair(id, BoundarySet{ position, boundary }));
}

void Camera::AddBoundarySet(int id, BoundarySet bSet)
{
    boundaries.insert(make_pair(id, bSet));
}

void Camera::SetCurrentBoundarySet(int id)
{
    currentBoundarySet = id;
}

BoundarySet Camera::GetBoundarySet(int id)
{
    if (boundaries.find(id) != boundaries.end())
        return boundaries.at(id);
    return BoundarySet::Empty();
}

void Camera::FreeBoundary()
{
    if (boundaryLocked == 1)
    {
        boundaryLocked = 0;
        lastBoundary = boundary;
        boundary.top = 0;
        DebugOut(L"Camera free: %f\n", lastBoundary.top);
    }
}

void Camera::LockBoundary()
{
    if (boundaryLocked == 0)
    {
        DebugOut(L"Camera locked: %f\n", lastBoundary.top);
        boundaryLocked = 1;
    }
}

void Camera::LockCamera()
{
    boundaryLocked = 2;
}

void Camera::UnlockCamera()
{
    boundaryLocked = 1;
}

void Camera::SetScrollMode(ScrollMode mode)
{
    scrollMode = mode;

    if (mode == ScrollMode::Automatic)
    {
        if (boundaries.find(currentBoundarySet) == boundaries.end()) return;
        auto curBSet = boundaries.at(currentBoundarySet);
        timer = 0;
        currentPathNode = 0;
        startPosition = position = curBSet.path.at(0);
    }
}

ScrollMode Camera::GetScrollMode()
{
    return scrollMode;
}

void Camera::Initialize()
{
    if (initialized) return;
    mapData = map->GetMapData();
    if (mapData == nullptr) { initialized = false; return; }

    layers = mapData->GetLayers();
    tilesets = mapData->GetTilesets();

    tileWidth = mapData->GetTileWidth();
    tileHeight = mapData->GetTileHeight();

    width = (int)(viewportSize.x / tileWidth);
    height = (int)(viewportSize.y / tileHeight);

    mapWidth = mapData->GetMapWidthInTiles();
    mapHeight = mapData->GetMapHeightInTiles();

    initialized = true;
}

void Camera::TargetingMode()
{
    if (target == nullptr) return;

    auto dt = Game::FixedDeltaTime() * Game::GetTimeScale() * 0.001f;

    auto pos = this->position;
    auto targetPos = target->GetTransform().Position;
    auto targetOnViewport = WorldToViewport(targetPos);

    auto finalPos = Vector2(targetPos.x - viewportSize.x * targetPivot.x, targetPos.y - viewportSize.y * targetPivot.y + bottomOffset);
    pos = finalPos;

    if (pos.x < boundary.left) pos.x = boundary.left;
    if (pos.x > boundary.right - viewportSize.x) pos.x = boundary.right - viewportSize.x;
    if (pos.y < boundary.top + bottomOffset) pos.y = boundary.top + bottomOffset;
    if (pos.y > boundary.bottom - viewportSize.y + bottomOffset) pos.y = boundary.bottom - viewportSize.y + bottomOffset;

    if (boundaryLocked > 0)
    {
        auto val = Mathf::Min(lastBoundary.top + bottomOffset, lastBoundary.bottom - viewportSize.y + bottomOffset);
        if (pos.y >= val)
        {
            // DebugOut(L"Cam: %f, %f\n", pos.y, val);
            boundary = lastBoundary;
        }
    }

    SetPosition(pos);
}

void Camera::AutoscrollingMode()
{
    if (boundaries.find(currentBoundarySet) == boundaries.end()) return;
    auto curBSet = boundaries.at(currentBoundarySet);
    if (currentPathNode >= curBSet.path.size()) return;

    auto dt = Game::DeltaTime() * Game::GetTimeScale() * 0.001f;
    timer += dt;

    auto dest = curBSet.path.at(currentPathNode);

    if (Mathf::Magnitude(dest - position) > 0.1f)
    {
        position = Mathf::Lerp(startPosition, dest, timer * curBSet.pathSpeed);
    }
    else
    {
        currentPathNode++;
        timer = 0;
        startPosition = position = dest;

        if (currentPathNode < curBSet.path.size())
            dest = curBSet.path.at(currentPathNode);
    }
}
