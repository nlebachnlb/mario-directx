#include "Text.h"

Text::Text()
{
	font = nullptr;
	spacing = -4;
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
		x += visual->GetSpriteWidth() + spacing;
	}
}

void Text::SetContent(std::string text)
{
	//OutputDebugStringW(ToLPCWSTR(text + "\n"));
	this->text = text;
	for (auto c : text) c = toupper(c);
}

std::string Text::GetContent()
{
	return this->text;
}

void Text::SetSpacing(int spacing)
{
	this->spacing = spacing;
}

void Text::SetFont(Font* font)
{
	this->font = font;
}
