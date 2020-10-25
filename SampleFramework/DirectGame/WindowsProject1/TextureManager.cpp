#include "TextureManager.h"
#include "Consts.h"
#include "Utils.h"

LPDIRECT3DTEXTURE9 TextureManager::Add(std::string id, LPCWSTR texPath, D3DCOLOR transparentColor)
{
	LPDIRECT3DTEXTURE9 texture = LoadTexture(texPath);
	textures.insert(make_pair(id, texture));
	return texture;
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture(std::string id)
{
	return textures.at(id);
}

LPDIRECT3DTEXTURE9 TextureManager::LoadTexture(LPCWSTR texPath)
{
	Debug::Log("Texture manager: Load texture");

	D3DXIMAGE_INFO info;
	LPDIRECT3DTEXTURE9 texture;

	auto result = D3DXGetImageInfoFromFileW(texPath, &info);
	auto d3ddev = Game::GetInstance().GetD3DDevice();

	if (result != D3D_OK) return NULL;

	result = D3DXCreateTextureFromFileExW(
		d3ddev, texPath, info.Width, info.Height,
		1, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
		D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 255, 255),
		&info, NULL, &texture
	);

	if (result != D3D_OK) return NULL;
	return texture;
}

void TextureManager::Initialization()
{
	auto root = Game::GetInstance();
	// Mario texture
	Add(TEXTURE_MARIO, ToLPCWSTR(root.GetSourcePathOf(CATEGORY_TEXTURE, TEXTURE_MARIO)), D3DCOLOR_XRGB(255, 255, 255));
}
