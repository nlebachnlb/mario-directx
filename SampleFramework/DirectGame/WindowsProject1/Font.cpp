#include "Font.h"
#include "Game.h"

Font::Font()
{
    spaceCharacterWidth = DEFAULT_SPACE_WIDTH;
}

void Font::Import(std::vector<FontSprite> fontSprites)
{
    dictionary.clear();
    auto sprMng = Game::GetInstance().GetService<SpriteManager>();
    for (auto fSpr : fontSprites)
        dictionary.insert(std::make_pair(fSpr.character, sprMng->Get(fSpr.spriteId)));
}

Sprite Font::GetChar(char c)
{
    if (!('0' <= c && c <= '9') && !('A' <= c && c <= 'Z')) return nullptr;
    return dictionary.at(c);
}

int Font::GetSpaceWidth()
{
    return this->spaceCharacterWidth;
}
