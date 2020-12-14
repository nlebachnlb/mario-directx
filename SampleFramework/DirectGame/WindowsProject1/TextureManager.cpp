#include "TextureManager.h"
#include "Consts.h"
#include "Utils.h"

LPDIRECT3DTEXTURE9 TextureManager::Add(std::string id, LPCWSTR texPath, D3DCOLOR transparentColor)
{
	LPDIRECT3DTEXTURE9 texture = LoadTexture(texPath, transparentColor);
	textures.insert(make_pair(id, texture));
	return texture;
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture(std::string id)
{
	return textures.at(id);
}

LPDIRECT3DTEXTURE9 TextureManager::LoadTexture(LPCWSTR texPath, D3DCOLOR transparentColor)
{
	DebugOut(L"Texture manager: Load texture: %d\n", transparentColor);

	D3DXIMAGE_INFO info;
	LPDIRECT3DTEXTURE9 texture;

	auto result = D3DXGetImageInfoFromFileW(texPath, &info);
	auto d3ddev = Game::GetInstance().GetD3DDevice();

	if (result != D3D_OK) return NULL;

	result = D3DXCreateTextureFromFileExW(
		d3ddev, texPath, info.Width, info.Height,
		1, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
		D3DX_DEFAULT, D3DX_DEFAULT, transparentColor,
		&info, NULL, &texture
	);

	if (result != D3D_OK) return NULL;
	return texture;
}

void TextureManager::Initialization()
{
	auto root = Game::GetInstance();
	// Mario texture
	Add(TEXTURE_MARIO,		ToLPCWSTR(root.GetSourcePathOf(CATEGORY_TEXTURE, TEXTURE_MARIO)), DEFAULT_TRANSPARENT_COLOR);
	Add(TEXTURE_ENEMY,		ToLPCWSTR(root.GetSourcePathOf(CATEGORY_TEXTURE, TEXTURE_ENEMY)), D3DCOLOR_XRGB(252, 252, 252));
	Add(TEXTURE_MISC,		ToLPCWSTR(root.GetSourcePathOf(CATEGORY_TEXTURE, TEXTURE_MISC)), D3DCOLOR_XRGB(254, 254, 254));
	Add(TEXTURE_FIREBALL,	ToLPCWSTR(root.GetSourcePathOf(CATEGORY_TEXTURE, TEXTURE_FIREBALL)), DEFAULT_TRANSPARENT_COLOR);
	Add(TEXTURE_EFFECT,		ToLPCWSTR(root.GetSourcePathOf(CATEGORY_TEXTURE, TEXTURE_EFFECT)), DEFAULT_TRANSPARENT_COLOR);
	Add(TEXTURE_PIPE,		ToLPCWSTR(root.GetSourcePathOf(CATEGORY_TEXTURE, TEXTURE_PIPE)), DEFAULT_TRANSPARENT_COLOR);
	Add(TEXTURE_HUD,		ToLPCWSTR(root.GetSourcePathOf(CATEGORY_TEXTURE, TEXTURE_HUD)), D3DCOLOR_XRGB(252, 252, 252));
	Add(TEXTURE_BOX,		ToLPCWSTR(root.GetSourcePathOf(CATEGORY_TEXTURE, TEXTURE_BOX)), D3DCOLOR_XRGB(255, 255, 255));
	Add(TEXTURE_REWARD,		ToLPCWSTR(root.GetSourcePathOf(CATEGORY_TEXTURE, TEXTURE_REWARD)), DEFAULT_TRANSPARENT_COLOR);
}
