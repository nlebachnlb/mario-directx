#pragma once
#include <unordered_map>
#include "Sprite.h"
#include <vector>

struct FontSprite
{
	char character;
	std::string spriteId;
};

class Font
{
public:
	void Import(std::vector<FontSprite> fontSprites);
	Sprite GetChar(char c);
protected:
	std::unordered_map<char, Sprite> dictionary;
};

