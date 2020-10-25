#include "Sprite.h"
#include "Game.h"
#include "tinyxml.h"
#include "Consts.h"

CSprite::CSprite(string id, int left, int top, int width, int height, Texture2D texture)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	this->texture = texture;
}

void CSprite::Draw(float x, float y, Vector2 scale, float rotation)
{
	int right = left + width;
	int bottom = top + height;

	Game::GetInstance().DrawTexture(x, y, width / 2, height / 2, texture, left, top, right, bottom, scale, rotation);
}

void CSprite::SetSourceRect(int left, int top, int width, int height)
{
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
}

void CSprite::SetSourceRect(RECT rect)
{
	SetSourceRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}

void SpriteManager::Initialization()
{
	// Load mario sprites
	LoadSpriteFile(Game::GetInstance().GetSourcePathOf(CATEGORY_SPRITE, DB_SPRITE_MARIO));
}

bool SpriteManager::LoadSpriteFile(std::string path)
{
	OutputDebugStringW(ToLPCWSTR(path.c_str()));
	TiXmlDocument document(path.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return false;
	}

	TiXmlElement* root = document.RootElement();
	TiXmlElement* texture = root->FirstChildElement();

	string textureID = texture->Attribute("id");
	Texture2D tex = Game::GetInstance().GetService<TextureManager>()->GetTexture(textureID);

	if (tex != nullptr)
		OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));

	for (TiXmlElement* node = texture->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
	{
		string spriteID = node->Attribute("id");
		int left, top, width, height;
		node->QueryIntAttribute("left", &left);
		node->QueryIntAttribute("top", &top);
		node->QueryIntAttribute("width", &width);
		node->QueryIntAttribute("height", &height);
		// OutputDebugStringW(ToLPCWSTR(spriteID + ':' + to_string(left) + ':' + to_string(top) + ':' + to_string(width) + ':' + to_string(height) + '\n'));
		Add(spriteID, 3 * left, 3 * top, 3 * width, 3 * height, tex);
	}
}

Sprite SpriteManager::Add(string id, int left, int top, int width, int height, Texture2D texture)
{
	Sprite spr = new CSprite(id, left, top, width, height, texture);
	sprites.insert(make_pair(id, spr));
	return spr;
}

Sprite SpriteManager::Get(string id)
{
	
	if (sprites.find(id) != sprites.end())
	{
		DebugOut(L"-Get sprite-");
		return sprites.at(id);
	}
	else
		return nullptr;
}

