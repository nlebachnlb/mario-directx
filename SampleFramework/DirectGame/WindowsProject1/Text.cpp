#include "Text.h"

Text::Text()
{
	font = nullptr;
}

void Text::PreRender()
{
}

void Text::Render()
{
	if (font == nullptr) return;

	int x = 0;
	for (auto ch : text)
	{
		auto visual = font->GetChar(ch);
		visual->Draw(rectTransform.Position.x + x, rectTransform.Position.y, 0, 0);
		x += visual->GetSpriteWidth();
	}
}

void Text::SetContent(std::string text)
{
	this->text = text;
}

std::string Text::GetContent()
{
	return this->text;
}

void Text::SetFont(Font* font)
{
	this->font = font;
}
