#pragma once
#include "Sprite.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

class AnimationFrame
{
public:
	AnimationFrame(Sprite sprite, DWORD time, D3DXVECTOR2 pos)
	{
		this->sprite = sprite; this->time = time; this->position = pos;
	}
	DWORD GetTime() { return time; }
	Sprite GetSprite() { return sprite; }
	D3DXVECTOR2 GetPosition() { return position; }
private:
	Sprite sprite;
	DWORD time;
	D3DXVECTOR2 position;
};
typedef AnimationFrame* AnimFrame;

