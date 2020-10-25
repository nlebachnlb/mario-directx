#include "Tile.h"
#include "Game.h"

void CTile::Draw(float x, float y, Vector2 scale, float rotation)
{
	int right = left + width;
	int bottom = top + height;

	Game::GetInstance().DrawTexture(x, y, 0, 0, texture, left, top, right, bottom);
}
