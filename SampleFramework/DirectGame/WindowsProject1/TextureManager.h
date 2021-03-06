#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "Game.h"
#include "Service.h"
#include <unordered_map>
#include "Debug.h"

#define DEFAULT_TRANSPARENT_COLOR D3DCOLOR_XRGB(255, 255, 255)
#define REWARD_TRANSPARENT_COLOR D3DCOLOR_XRGB(21, 168, 235)

class TextureManager : public Service
{
public:
	LPDIRECT3DTEXTURE9 Add(std::string id, LPCWSTR texPath, D3DCOLOR transparentColor);
	void Release(std::string id);
	LPDIRECT3DTEXTURE9 GetTexture(std::string id);
	LPDIRECT3DTEXTURE9 LoadTexture(LPCWSTR texPath, D3DCOLOR transparentColor = DEFAULT_TRANSPARENT_COLOR);
	void Initialization();
private:
	std::unordered_map<std::string, LPDIRECT3DTEXTURE9> textures;
};

