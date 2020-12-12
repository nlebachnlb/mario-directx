#pragma once
#include <unordered_map>
#include "Sprite.h"
#include <vector>

const int DEFAULT_SPACE_WIDTH = 24;

struct FontSprite
{
	char character;
	std::string spriteId;
};

class Font
{
public:
	Font();
	void Import(std::vector<FontSprite> fontSprites);
	Sprite GetChar(char c);
	int GetSpaceWidth();
protected:
	std::unordered_map<char, Sprite> dictionary;
	int spaceCharacterWidth;
};

