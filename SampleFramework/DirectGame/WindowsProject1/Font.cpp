#include "Font.h"
#include "Game.h"

void Font::Import(std::vector<FontSprite> fontSprites)
{
    dictionary.clear();
    auto sprMng = Game::GetInstance().GetService<SpriteManager>();
    for (auto fSpr : fontSprites)
        dictionary.insert(std::make_pair(fSpr.character, sprMng->Get(fSpr.spriteId)));
}

Sprite Font::GetChar(char c)
{
    return dictionary.at(c);
}
