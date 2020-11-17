#include "Sprite.h"
#include "Game.h"
#include "tinyxml.h"
#include "Consts.h"

CSprite::CSprite(string id, int left, int top, int width, int height, Texture2D texture, int xPivot, int yPivot)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	this->texture = texture;
	this->xPivot = xPivot;
	this->yPivot = yPivot;
}

void CSprite::Draw(float x, float y, Vector2 scale, float rotation, int alpha)
{
	int right = left + width;
	int bottom = top + height;
	
	int px = xPivot < 0 ? width / 2 : xPivot;
	int py = yPivot < 0 ? height / 2 : yPivot;

	Game::GetInstance().DrawTexture(x, y, px, py, texture, left, top, right, bottom, scale, rotation, alpha);
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
	LoadSpriteFile(Game::GetInstance().GetSourcePathOf(CATEGORY_SPRITE, DB_SPRITE_ENEMY));
	LoadSpriteFile(Game::GetInstance().GetSourcePathOf(CATEGORY_SPRITE, DB_SPRITE_MISC));
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

	for (TiXmlElement* texture = root->FirstChildElement("Textures"); texture != nullptr; texture = texture->NextSiblingElement("Textures"))
	{
		string textureID = texture->Attribute("textureId");
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

			int px, py;
			if (node->QueryIntAttribute("xPivot", &px) != TIXML_SUCCESS)
				px = -1;
			else
				px *= 3;
			if (node->QueryIntAttribute("yPivot", &py) != TIXML_SUCCESS)
				py = -1;
			else
				py *= 3;

			// OutputDebugStringW(ToLPCWSTR(spriteID + ':' + to_string(left) + ':' + to_string(top) + ':' + to_string(width) + ':' + to_string(height) + '\n'));
			Add(spriteID, 3 * left, 3 * top, 3 * width, 3 * height, tex, px, py);
		}
	}
}

Sprite SpriteManager::Add(string id, int left, int top, int width, int height, Texture2D texture, int xPivot, int yPivot)
{
	Sprite spr = new CSprite(id, left, top, width, height, texture, xPivot, yPivot);
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

