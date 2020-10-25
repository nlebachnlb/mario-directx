#pragma once
#include "Sprite.h"

class CTile : public CSprite
{
public:
	CTile(std::string id, int left, int top, int width, int height, Texture2D texture) :
		CSprite(id, left, top, width, height, texture)
	{

	}

	void Draw(float x, float y, Vector2 scale = Vector2(1.0f, 1.0f), float rotation = 0.0f) override;
};

typedef CTile* Tile;
