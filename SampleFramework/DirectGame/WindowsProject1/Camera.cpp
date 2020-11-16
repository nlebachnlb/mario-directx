#include "Camera.h"
#include "Consts.h"
#include "Game.h"
#include <cmath>

Camera::Camera()
{
    auto configs = Game::GetInstance().GetGlobalConfigs();
	this->viewportSize = Vector2(configs.screenWidth, configs.screenHeight);
	this->position = VectorZero();
    this->map = nullptr;
    this->targetPivot = Vector2(0.5f, 0.3f);
    initialized = false;
}

Camera::Camera(Vector2 startPosition, Vector2 viewportSize)
{
	this->position = startPosition;
	this->viewportSize = viewportSize;
    initialized = false;
}

Camera::~Camera()
{
    if (map != nullptr) delete map;
}

void Camera::Update()
{
    if (map == nullptr) map = Game::GetInstance().GetService<GameMap>();

    auto pos = this->position;
    
    pos.x = target->GetTransform().Position.x - viewportSize.x * targetPivot.x;
    pos.y = target->GetTransform().Position.y - viewportSize.y * targetPivot.y;

    if (pos.x < boundary.left) pos.x = boundary.left;
    if (pos.x > boundary.right - viewportSize.x) pos.x = boundary.right - viewportSize.x;
    if (pos.y < boundary.top) pos.y = boundary.top;
    if (pos.y > boundary.bottom - viewportSize.y) pos.y = boundary.bottom - viewportSize.y;

    SetPosition(pos);
}

void Camera::Render()
{
    if (map == nullptr) map = Game::GetInstance().GetService<GameMap>();
    if (!initialized) Initialize();

    Vector2 translation = -1 * GetPosition();

    for (auto l_data : *layers)
    {
        auto layer = l_data.second;
        if (layer->IsVisible() == false) continue;

        for (auto t_data : *tilesets)
        {
            auto tileset = t_data.second;

            int tilesetWidth = tileset->GetImageWidth() / tileWidth;
            int tilesetHeight = tileset->GetImageHeight() / tileHeight;

            for (int u = 0; u <= height + 1; ++u)
            {
                for (int v = 0; v <= width + 1; ++v)
                {
                    auto camPos = GetPosition();
                    int xGrid = (int)(camPos.x / tileWidth + v) % mapWidth;
                    int yGrid = (int)(camPos.y / tileHeight + u) % mapHeight;

                    int tileId = layer->GetTileID(xGrid, yGrid);
                    int tilesetId = mapData->GetTilesetIdFromTileId(tileId);

                    if (tilesetId == -1) continue;

                    int y = (tileId - tilesetId) / tilesetWidth;
                    int x = (tileId - tilesetId) - y * tilesetWidth;

                    Tile tile = map->GetTileset(tilesetId);
                    tile->SetSourceRect(x * tileWidth, y * tileHeight, tileWidth, tileHeight);

                    Vector2 finalPosition(xGrid * tileWidth, yGrid * tileHeight);
                    finalPosition = finalPosition + translation;
                    tile->Draw(finalPosition.x, finalPosition.y);
                }
            }
        }
    }
}

bool Camera::RectInsideCameraView(RECT rect)
{
	auto rectWidth = rect.right - rect.left;
	auto rectHeight = rect.bottom - rect.top;

	return rect.left >= position.x - rectWidth && rect.right <= position.x + viewportSize.x + rectWidth &&
			rect.top >= position.y - rectHeight && rect.bottom <= position.y + viewportSize.y + rectHeight;
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
    this->boundary = boundary;
}

void Camera::AddBoundarySet(int id, Vector2 position, RectF boundary)
{
    boundaries.insert(make_pair(id, BoundarySet{ position, boundary }));
}

void Camera::AddBoundarySet(int id, BoundarySet bSet)
{
    boundaries.insert(make_pair(id, bSet));
}

BoundarySet Camera::GetBoundarySet(int id)
{
    if (boundaries.find(id) != boundaries.end())
        return boundaries.at(id);
    return BoundarySet::Empty();
}

void Camera::Initialize()
{
    if (initialized) return;
    mapData = map->GetMapData();
    if (map == nullptr) { initialized = false; return; }

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
