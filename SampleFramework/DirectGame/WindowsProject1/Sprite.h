#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include <d3d9.h>
#include <d3dx9.h>
#include "Utils.h"
#include "Service.h"
#include <unordered_map>
#include "Transform.h"

class CSprite
{
public:
	CSprite(std::string id, int left, int top, int width, int height, Texture2D texture, int xPivot = -1, int yPivot = -1);
	virtual void Draw(float x, float y, Vector2 scale = Vector2(1.0f, 1.0f), float rotation = 0.0f);
	void SetSourceRect(int left, int top, int width, int height);
	void SetSourceRect(RECT rect);
protected:
	std::string id;
	int top, left;
	int width, height;
	int xPivot, yPivot;

	Texture2D texture;
};

typedef CSprite* Sprite;

class SpriteManager : public Service
{
public:
	void Initialization();
	bool LoadSpriteFile(std::string path);
	Sprite Add(std::string id, int left, int top, int width, int height, Texture2D texture, int xPivot, int yPivot);
	Sprite Get(std::string id);

private:
	std::unordered_map<std::string, Sprite> sprites;
};
#endif